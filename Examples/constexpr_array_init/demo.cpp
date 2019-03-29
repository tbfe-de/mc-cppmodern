// http://coliru.stacked-crooked.com/a/759ceaa722afc1ae

#include <array>
#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;

template<int N>
struct primes {
    int data[N] = {2, 3};
    constexpr primes() {
        int n = 2;
        while (n < N) {
            int candidate = data[n-1] + 2;
            for (int t = 0; t < n; ++t) {
                if (candidate % data[t] == 0) {
                    candidate += 2; t = 0;
                }
            }
            data[n++] = candidate;
        }
    }
};

template<> struct primes<1> {
    int data[1] = {2};
};

int main() {
    constexpr primes<12> ptab{};
    for (auto i : ptab.data) std::cout << i << ' ';
    std::cout << std::endl;
}
