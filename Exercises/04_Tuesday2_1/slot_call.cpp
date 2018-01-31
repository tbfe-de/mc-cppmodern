#include "pxtn.h"

#include <functional>
#include <sstream>
#include <vector>

std::ostringstream output;

void my_func(const char*, int) {
	output << "my_func\n";
}

struct MyFunctor {
	void operator()(const char* cp, int) {
		output << "MyFunctor::operator()(" << cp << ")\n";
	}
};

PN_(Testing 'Publisher - Subscriber' POC Implementation)

int main() {
	std::vector<std::function<void(const char*, int)>> slot_call;

	slot_call.emplace_back(my_func);
	slot_call.emplace_back(MyFunctor{});
	slot_call.emplace_back([](const char *arg1, int arg2) {
        	output << arg1;
        	while(arg2 > 0)
			output << " ... " << arg2--;
		output << '\n';
	});

	// --------------------------------------------------
	for (const auto& f : slot_call) f("hello, world", 3);
	// --------------------------------------------------
	PX_("my_func"					"\n"
	    "MyFunctor::operator()(hello, world)"	"\n"
	    "hello, world ... 3 ... 2 ... 1"		"\n",
            output.str());

	slot_call.at(0) = nullptr;
        slot_call.at(2) = nullptr;

	output.str(std::string{});
	// --------------------------------------------------
	for (const auto& f : slot_call)
		if (f) f("xyz", 0);
	// --------------------------------------------------
	PX_("MyFunctor::operator()(xyz)\n", output.str());

	slot_call.at(0) = [](const char* x, int y) {
				output << "x=" << x << ", "
                                       << "y=" << y << ";\n";
        };

	output.str(std::string{});
	// --------------------------------------------------
	for (const auto& f : slot_call)
		if (f) f("abc", -1);
	// --------------------------------------------------
	PX_("x=abc, y=-1;\n"
            "MyFunctor::operator()(abc)\n", output.str());

}
