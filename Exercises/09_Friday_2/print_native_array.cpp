#include <cstddef>
#include <iostream>

template<typename T>
void print_native_array_impl(const T arr[], std::size_t cnt) {
	for (auto i = std::size_t{0}; i < cnt; ++i)
		std::cout << i << ": " << arr[i] << std::endl;
}

template<typename T, std::size_t N>
inline
void print_native_array(T (&arg)[N]) {
	print_native_array_impl(arg, N);
}

const char *nums[] = { "one", "two", "three" };
int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
int squares[] = { 1, 4, 9, 16, 25, 36, 49, 64, 81, 100 };

int main() {
	std::cout << ":: Numbers" << std::endl;
	print_native_array(nums);

	std::cout << ":: Primes" << std::endl;
	print_native_array(primes);

	std::cout << ":: Squares" << std::endl;
	print_native_array(squares);
}
