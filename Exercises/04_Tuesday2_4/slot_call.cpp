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

PN_(Step 4 Testing of class `CallSlots`)

int main() {
	CallSlots subs;

	const auto reg =  subs.register_call(
				[](const char *arg1, int arg2) {
					output << arg1;
					while(arg2 > 0)
						output << " ... " << arg2--;
					output << '\n';
				});

	// --------------------------------------------------
	subs.notify_all("hello, world", 3);
	// --------------------------------------------------
	PX_("hello, world ... 3 ... 2 ... 1"	"\n",
            output.str());

	const auto second_wrapped = subs.register_call(
		[old = subs.unregister_call(reg)](const char* _1, int _2) {
			output << "!!--> STARTING\n";
			old(_1, _2);
			output << "!!--> RETURNED\n";
		});

	output.str(std::string{});
	// --------------------------------------------------
	subs.notify_all("hello, world", 3);
	// --------------------------------------------------
	PX_("!!--> STARTING"			"\n"
	    "hello, world ... 3 ... 2 ... 1"	"\n"
	    "!!--> RETURNED"			"\n",
	    output.str());
}
