// Simple Implementation of "Publisher - Subscriber"
//
// (rather just a "proof of concept" in that state)

#include <algorithm>
#include <functional>
#include <vector>

class slot_call {
private:
	using callback_type = std::function<void(const char*)>;
	using callback_list = std::vector<callback_type>;
	callback_list v;
public:
	void signal_all(const char *text) {
		for (const auto &e : v)
			if (e) e(text);
	}
	int register_callback(callback_type cb) {
		const auto f = std::find(v.begin(), v.end(), nullptr);
		const auto callback_id = f - v.begin();
		v.insert(f, cb);
		return callback_id;
	}
	void unregister_callback(int callback_id) {
		v.at(callback_id) = nullptr;
	}
};

#include <iostream>

struct Foo {
	void operator()(const char *t) {
		std::cout << "functor Foo: " << t << std::endl;
       	}
};
void bar(const char* t) {
	std::cout << "function bar: " << t << std::endl;
}

int main() {
	slot_call s;
	const auto cb1 = s.register_callback(Foo{});
	const auto cb2 = s.register_callback(bar);
	const auto cb3 = s.register_callback([](const char* t) {
				std::cout << "first lambda: " << t << std::endl;
				});
	// -------------------
	s.signal_all("start");
	// -------------------

	s.unregister_callback(cb2);

	// -------------------
	s.signal_all("middle");
	// -------------------

	const auto cb4 = s.register_callback([](const char* t) {
				std::cout << "second lambda: " << t << std::endl;
				});
	// -------------------
	s.signal_all("end");
	// -------------------

}
