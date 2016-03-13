// http://coliru.stacked-crooked.com/a/ee8ae6898261b662

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))


#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <set>
#include <sstream>

std::ostream& operator<<(std::ostream& lhs, const std::vector<int> &rhs) {
   std::copy(std::begin(rhs), std::end(rhs),
             std::ostream_iterator<int>(lhs, " "));
   return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const std::forward_list<int> &rhs) {
   std::copy(std::begin(rhs), std::end(rhs),
             std::ostream_iterator<int>(lhs, " "));
   return lhs;
}

void cpp11_new_algorithms() {
    using namespace std::placeholders;
#if 1
    std::vector<int> test_input = {3, 7, 5, 2, 9, 25, 8, 17, 3};
    std::vector<int> multiples_removed;
#else
    std::forward_list<int> test_input = {3, 7, 5, 2, 9, 25, 8, 17, 3};
    std::forward_list<int> result;
#endif
    auto is_divisible = [](int v, int n) {return (v%n == 0);};
    std::set<int> divisors;

    PX(test_input);
#if 1
    std::copy_if(test_input.cbegin(), test_input.cend(),
                 std::back_inserter(multiples_removed),
                 [is_divisible, &divisors](int v) -> bool {
                 //  auto v_is_divisible = std::bind(is_divisible, v, _1);
                     auto v_is_divisible = [v](int n) {return (v%n == 0);};
                     const auto &cdivs = divisors;
                     if (std::find_if(cdivs.begin(), cdivs.end(),
                                      v_is_divisible) != cdivs.end())
                         return false;
                     divisors.insert(v);
                     return true;
                 });
#else
    std::remove_copy_if(test_input.cbegin(), test_input.cend(),
                 std::back_inserter(multiples_removed),
                 [is_divisible, &divisors](int v) -> bool {
                 //  auto v_is_divisible = std::bind(is_divisible, v, _1);
                     auto v_is_divisible = [v](int n) {return (v%n == 0);};
                     if (std::find_if(divisors.cbegin(), divisors.cend(),
                                      v_is_divisible) != divisors.cend())
                         return true
                     divisors.insert(v);
                     return false;
                 });
#endif
    PX(multiples_removed);
 
    using BFI = std::function<bool(int)>;

    auto divides_25 =     [](int n) {return (25%n == 0);};
//  auto divides_25 = BFI{[](int n) {return (25%n == 0);}};
//  auto divides_25 =     std::bind(is_divisable, 25, _1);
//  auto divides_25 = BFI{std::bind(is_divisable, 25, _1)};

//  auto not_divides_25 =     [](int n) {return (25%n != 0);};
//  auto not_divides_25 = BFI{[](int n) {return (25%n != 0);}};
//  auto not_divides_25 =     std::not1(divides_25);
//  auto not_divides_25 = BFI{std::not1(divides_25)};
    auto not_divides_25 =     std::not1(BFI{divides_25});

    PX(std::any_of(test_input.cbegin(), test_input.cend(), divides_25));
    PX(!std::none_of(test_input.cbegin(), test_input.cend(), divides_25));
    PX(!std::all_of(test_input.cbegin(), test_input.cend(), not_divides_25));

    PX(std::any_of(multiples_removed.cbegin(), multiples_removed.cend(), not_divides_25));
    PX(!std::none_of(multiples_removed.cbegin(), multiples_removed.cend(), not_divides_25));
    PX(!std::all_of(multiples_removed.cbegin(), multiples_removed.cend(), divides_25));
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    cpp11_new_algorithms();
}
