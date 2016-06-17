#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

auto abs_cmp_external_function(double left, double right) -> bool;

auto abs_cmp_inline_function(double left, double right) {
#if 1
    return left < right;
#else
    return (std::abs(left) < std::abs(right)
        || (std::abs(left) == std::abs(right) && left < right));
#endif
}

auto abs_cmp_named_lambda =
[](double left, double right) {
#if 1
    return left < right;
#else
    return (std::abs(left) < std::abs(right)
        || (std::abs(left) == std::abs(right) && left < right));
#endif
};

std::function<bool(double, double)> abs_cmp_std_function =
[](double left, double right) {
#if 1
    return left < right;
#else
    return (std::abs(left) < std::abs(right)
        || (std::abs(left) == std::abs(right) && left < right));
#endif
};

int main() {
    namespace sc = std::chrono;

    std::vector<double> data;
    auto os = std::ostream_iterator<double>(std::cout, "; ");

    std::generate_n(std::back_inserter(data), 2e5,
                    [n = 1]() mutable {return 1e5/n++;});
    std::copy(data.begin(), data.begin()+5, os); std::cout << " ... ";
    std::copy(data.end()-5, data.end(), os); std::cout << '\n';
    {
        std::random_shuffle(data.begin(), data.end());
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        auto start = sc::high_resolution_clock::now();
        std::sort(data.begin(), data.end(), abs_cmp_named_lambda);
        auto delta = sc::high_resolution_clock::now() - start;
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        std::cout << "named lambda: " << delta.count() << "\n\n";
    }
    {
        std::random_shuffle(data.begin(), data.end());
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        auto start = sc::high_resolution_clock::now();
        std::sort(data.begin(), data.end());
        auto delta = sc::high_resolution_clock::now() - start;
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        std::cout << "sort default compare: " << delta.count() << "\n\n";
    }
    {
        std::random_shuffle(data.begin(), data.end());
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        auto start = sc::high_resolution_clock::now();
        std::sort(data.begin(), data.end(), abs_cmp_inline_function);
        auto delta = sc::high_resolution_clock::now() - start;
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        std::cout << "inline function: " << delta.count() << "\n\n";
    }
    {
        std::random_shuffle(data.begin(), data.end());
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        auto start = sc::high_resolution_clock::now();
        std::sort(data.begin(), data.end(), abs_cmp_external_function);
        auto delta = sc::high_resolution_clock::now() - start;
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        std::cout << "external function: " << delta.count() << "\n\n";
    }
    {
        std::random_shuffle(data.begin(), data.end());
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        auto start = sc::high_resolution_clock::now();
        std::sort(data.begin(), data.end(), abs_cmp_std_function);
        auto delta = sc::high_resolution_clock::now() - start;
        std::copy(data.begin(), data.begin()+5, os); std::cout << '\n';
        std::cout << "std::function: " << delta.count() << "\n\n";
    }

}
