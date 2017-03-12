#include <iostream>
#include <string>
#include <type_traits>

#if 1

template<typename T>
auto clear_if_has_clear(T arg) -> decltype(arg.size(), std::declval<T>()) {
	arg.size();
	std::cout << "... clear called ...\n";
}
void clear_if_has_clear(...)     {}

template<typename ArgType>
void do_it(ArgType arg) {
#if 1
	clear_if_has_clear(arg);
#else
	if (std::is_floating_point<ArgType>::value) {
		std::cout << "... floating type argument ...\n";
		return;
	}
	if (std::is_integral<ArgType>::value) {
		std::cout << "... integral type argument ...\n";
		// arg >>= 1;
		return;
	}
	if (std::is_same<ArgType, std::string>::value) {
		std::cout << "... string type argument ...\n";
		// arg.clear();
		return;
	}
	else {
		std::cout << "???\n";
		return;
	}
#endif
}
#else
enum class Sel { Invalid, Integral, Floating, String };
template<typename Type>
constexpr Sel select_by() {
	if (std::is_floating_point<Type>::value)
		return Sel::Floating;
	if (std::is_integral<Type>::value)
		return Sel::Integral;
	if (std::is_same<Type, std::string>::value)
		return Sel::String;
	return Sel::Invalid;
}

template<Sel, typename ArgType>
struct do_it_impl;

template<typename ArgType>
struct do_it_impl<Sel::Floating, ArgType> {
	inline
	void operator()(ArgType arg) {
		std::cout << "... floating type argument ...\n";
	}
};

template<typename ArgType>
struct do_it_impl<Sel::Integral, ArgType> {
	inline
	void operator()(ArgType arg) {
		std::cout << "... integral type argument ...\n";
		arg >>= 1;
	}
};

template<typename ArgType>
struct do_it_impl<Sel::String, ArgType> {
	inline
	void operator()(ArgType arg) {
		std::cout << "... string type argument ...\n";
		arg.clear();
	}
};

template<typename ArgType>
void do_it(ArgType arg) {
	do_it_impl<select_by<ArgType>(), ArgType>{}(arg);
}
#endif

int main() {
	using namespace std::literals::string_literals;
  	do_it(0.f);
}
