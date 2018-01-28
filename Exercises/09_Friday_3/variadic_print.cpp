#include <iostream>
#include <string>


void variadic_print_impl() {
    std::cout << std::endl;
}

template<typename T, typename... Ts>
void variadic_print_impl(T first, Ts ...rest) {
	std::cout << first << "; ";
	variadic_print_impl(rest...);
}

template<typename... Ts>
void variadic_print(Ts ...args) {
	variadic_print_impl(args...);
}

#include <initializer_list>
template<typename T>
void vlist_print(const std::initializer_list<T>& li) {
	for (const auto& e : li)
		std::cout << e << "; ";
	std::cout << std::endl;
}

int main() {
	std::cout.setf(std::ios::boolalpha);
	const std::string w("world");
	variadic_print(12.23, !true, "hello", w, "!!!!");

	vlist_print({1, 2, 3, 4, 5});
	vlist_print({1.2, 3.4, 5.0});
	//vlist_print({1.2, 3.4, 5});
	vlist_print<double>({1.2, 3.4, 5});
	//vlist_print<int>({1.2, 3.4, 5});
}
