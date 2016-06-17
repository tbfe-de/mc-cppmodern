#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

int main() {
    std::vector<int> data;
    auto os = std::ostream_iterator<int>(std::cout, "; ");

    data.resize(24);
    std::copy(data.begin(), data.end(), os); std::cout << '\n';

    std::iota(data.begin(), data.end(), -int{data.size()/3});
    assert(data.size() >= 5);
    std::copy(data.begin()+3, data.end()-2, os); std::cout << '\n';
    std::copy(data.begin(), data.end(), os); std::cout << '\n';

    std::random_shuffle(data.begin(), data.end());
    std::copy(data.begin(), data.end(), os); std::cout << '\n';

    std::sort(data.begin(), data.end());
    std::copy(data.begin(), data.end(), os); std::cout << '\n';

    std::sort(data.begin(), data.end(), [](int left, int right) {return (left > right);});
    std::copy(data.begin(), data.end(), os); std::cout << '\n';

    // auto abs_cmp = [](int left, int right) {
    std::function<bool(int, int)> abs_cmp = [](int left, int right) {
                                                return (std::abs(left) < std::abs(right)
                                                    || (std::abs(left) == std::abs(right) && left < right));};
    std::sort(data.begin(), data.end(), abs_cmp);
    std::copy(data.begin(), data.end(), os); std::cout << '\n';

}
