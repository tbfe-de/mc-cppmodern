#include <iostream>
#include <string>

void print_helper() {
	std::cout << "done" << std::endl;
}

template<typename T, typename... Ts>
void print_helper(const T& arg, Ts... rest_args) {
	std::cout << arg << " ";
	print_helper(rest_args...);
}

template<typename... Ts>
void print_helper(const std::string& arg, Ts... rest_args) {
	std::cout << '"' << arg << '"' << " ";
	print_helper(rest_args...);
}

template <typename... Ts>
void print_all(Ts... args) {
	print_helper(args...);
}

int main() {
	std::string greet{"hello, world"};
	std::cout.setf(std::ios::boolalpha);
	print_all(123, true, 5.67, greet, false);
}
