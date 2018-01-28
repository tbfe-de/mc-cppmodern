#include <algorithm>
#include <array>
#include <deque>
#include <forward_list>
#include <iostream>
#include <iomanip>
#include <initializer_list>
#include <iterator>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

#include <boost/type_index.hpp>

template<typename InIt, typename OutIt>
OutIt no_dupes_copy(InIt b, InIt e, OutIt t) {
    std::set<typename std::iterator_traits<InIt>::value_type> s;
    return  std::copy_if(b, e, t, [&s](const auto& e) { return s.insert(e).second; });
}

template<typename T, std::size_t N>
void run_test(T (&c)[N]) {
    std::ostream_iterator<T> osit(std::cout, "; ");
    std::cout << boost::typeindex::type_id_with_cvr<T[N]>();
    std::cout << "\n(in) <-- "; std::copy(std::cbegin(c), std::cend(c), osit);
    std::cout << "\n(out)--> "; no_dupes_copy(std::cbegin(c), std::cend(c), osit);
    std::cout << std::endl;
}

template<typename ContainerType>
void run_test(const ContainerType& c) {
    std::ostream_iterator<typename ContainerType::value_type> osit(std::cout, "; ");
    std::cout << boost::typeindex::type_id_with_cvr<ContainerType>();
    std::cout << "\n (in)<-- "; std::copy(std::cbegin(c), std::cend(c), osit);
    std::cout << "\n(out)--> "; no_dupes_copy(std::cbegin(c), std::cend(c), osit);
    std::cout << std::endl;
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    int data[] = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    run_test(data);

    run_test(std::vector<bool>{ true, false, false, true, !false, true, !true });

    run_test(std::deque<int>{ 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 });

    run_test(std::forward_list<int>{ 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 });

    run_test(std::set<int>{ 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 });
    run_test(std::multiset<int>{ 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 });
    run_test(std::unordered_set<int>{ 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 });
    run_test(std::unordered_multiset<int>{ 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 });
    run_test("ABC");
}
