#include "pxtn.h"

#include <algorithm>
#include <functional>
#include <map>
#include <random>
#include <sstream>

class CallSlots {
	using RandomEngine = std::mt19937;
public:
	using CallableId = RandomEngine::result_type;
        using CallableType = std::function<void(const char*, int)>;
        using CallableList = std::map<CallableId, CallableType>;
private:
        CallableList slots;
	CallableId generate_id() {
		static RandomEngine rand{std::random_device{}()};
		CallableId result;
		do {
			result = rand();
		} while (slots.count(result) > 0);
		return result;
	}
public:
        CallSlots() =default;
        auto register_call(const CallableType cb) -> CallableId {
		#if 0
		const auto id = generate_id();
		slots.emplace(id, cb);
		return id;
		#else
		return slots.emplace(generate_id(), cb).first->first;
		#endif
	}
        auto unregister_call(CallableId id) -> CallableType {
		const auto cbit = slots.find(id);
		if (cbit == slots.end())
			return nullptr; 
		auto cb = std::move(cbit->second);
		slots.erase(cbit);
		return std::move(cb);
	}
        void notify_all(const char* arg1, int arg2) {
		for (const auto& cb : slots) {
			const auto &f = cb.second;
			if (f) f(arg1, arg2);
		}

	}
};

std::ostringstream output;

void my_func(const char*, int) {
	output << "my_func\n";
}

struct MyFunctor {
	void operator()(const char* cp, int) {
		output << "MyFunctor::operator()(" << cp << ")\n";
	}
};

#include <vector>

std::string sortlines(std::string s) {
	std::istringstream is(s);
	std::string line;
	std::vector<std::string> tmp;
	while (std::getline(is, line))
		tmp.emplace_back(line);
	std::sort(tmp.begin(), tmp.end());
	std::string result;
	for (const auto& t : tmp)
		result.append(t + "\n");
	return result;
}

PN_(Step 3 Testing of class `CallSlots`)

int main() {
	CallSlots subscribers;

	const auto first =  subscribers.register_call(my_func);
	const auto second = subscribers.register_call(MyFunctor{});
	const auto third =  subscribers.register_call(
				[](const char *arg1, int arg2) {
					output << arg1;
					while(arg2 > 0)
						output << " ... " << arg2--;
					output << '\n';
				});

	// --------------------------------------------------
	subscribers.notify_all("hello, world", 3);
	// --------------------------------------------------
	PX_(sortlines(
	    "my_func"					"\n"
	    "MyFunctor::operator()(hello, world)"	"\n"
	    "hello, world ... 3 ... 2 ... 1"		"\n"),
            sortlines(output.str()));

	subscribers.unregister_call(first);
	subscribers.unregister_call(third);

	output.str(std::string{});
	// --------------------------------------------------
	subscribers.notify_all("xyz", 0);
	// --------------------------------------------------
	PX_("MyFunctor::operator()(xyz)\n", output.str());

	const auto fourth = subscribers.register_call(
				[](const char* x, int y) {
					output << "x=" << x << ", "
					       << "y=" << y << ";\n";
        			});

	output.str(std::string{});
	// --------------------------------------------------
	subscribers.notify_all("abc", -1);
	// --------------------------------------------------
	PX_(sortlines(
	    "x=abc, y=-1;\n"
            "MyFunctor::operator()(abc)\n"),
	    sortlines(output.str()));

	const auto second_wrapped = subscribers.register_call(
		[old = subscribers.unregister_call(second)](const char* _1, int _2) {
			output << "!!--> ";
			old(_1, _2);
		});

	output.str(std::string{});
	// --------------------------------------------------
	subscribers.notify_all("def", -1);
	// --------------------------------------------------
	PX_(sortlines(
	    "x=def, y=-1;\n"
            "!!--> MyFunctor::operator()(def)\n"),
	    sortlines(output.str()));
}

#include <cmath>

struct hit_specific_value {
	/*constexpr*/
	static long double propability(unsigned long long tries, int bits) {
		return (1-std::pow(1-1.L/(1uLL<<(bits-1)), tries));
	}
	~hit_specific_value() {
		std::cout.precision(10);
		for (const double N : {
				//	10uLL,
				//	100uLL,
					1'000uLL,
					1'000'000uLL,
					1'000'000'000uLL,
				// 	1'000'000'000'000uLL
			}) {
			std::cout << "hit specfic value in " << N << " attempts:\n"
				  << "    15 bit: " << 100*propability(N, 15) << " %\n"
				  << "    16 bit: " << 100*propability(N, 16) << " %\n"
				  << "    32 bit: " << 100*propability(N, 32) << " %\n"
				  << "    64 bit: " << 100*propability(N, 64) << " %\n";
		}
	}
} xxx; // <--- insert/remove global object to view from the above
