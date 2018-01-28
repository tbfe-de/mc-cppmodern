#include <iostream>

template<typename T, std::size_t N>
void print_native_array(T (&arg)[N]) {
	for (auto i = std::size_t{0}; i < N; ++i)
		std::cout << i << ": " << arg[i] << std::endl;
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
