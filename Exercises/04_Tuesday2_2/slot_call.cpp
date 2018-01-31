#include "pxtn.h"

#include <algorithm>
#include <functional>
#include <sstream>
#include <vector>

class CallSlots {
public:
        using CallableType = std::function<void(const char*, int)>;
        using CallableList = std::vector<CallableType>;
        using CallableId = CallableList::size_type;
private:
        CallableList slots;
public:
        CallSlots() =default;
        auto register_call(const CallableType cb) -> CallableId {
		const auto available = std::find(slots.begin(), slots.end(), nullptr);
		const auto inserted = slots.insert(available, cb);
		return inserted - slots.begin(); 
	}
        auto unregister_call(CallableId id) {
		auto cb = std::move(slots.at(id));
		slots.at(id) = nullptr;
		return std::move(cb);
	}
        void notify_all(const char* arg1, int arg2) {
		for (const auto& cb : slots) {
			if (cb) cb(arg1, arg2);
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

PN_(Step 2 Testing of class `CallSlots`)

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
	PX_("my_func"					"\n"
	    "MyFunctor::operator()(hello, world)"	"\n"
	    "hello, world ... 3 ... 2 ... 1"		"\n",
            output.str());

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
	PX_("x=abc, y=-1;\n"
            "MyFunctor::operator()(abc)\n", output.str());

}
