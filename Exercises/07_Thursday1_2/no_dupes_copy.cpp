#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <vector>
#if __cplusplus >= 201103L
#include <array>
#include <forward_list>
#include <initializer_list>
#endif

template<typename InIt, typename OutIt>
OutIt no_dupes_copy(InIt b, InIt e, OutIt t) {
    typedef std::set<typename std::iterator_traits<InIt>::value_type> SetType;
    SetType s;
    while (b != e) {
        const typename SetType::value_type v = *b++;
        if (s.insert(v).second)
            *t++ = v;
    }
    return t;
}

int main() {
    std::ostream_iterator<int> osit(std::cout, "; ");

    int data1[] = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    no_dupes_copy(data1, data1 + (sizeof data1 / sizeof *data1), osit);
    std::cout << std::endl;

    #if __cplusplus >= 201103L

    std::vector<int> data2 = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    no_dupes_copy(std::begin(data2), std::end(data2), osit);
    std::cout << std::endl;

    std::list<int> data3 = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    no_dupes_copy(std::begin(data3), std::end(data3), osit);
    std::cout << std::endl;

    std::deque<int> data4 = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    no_dupes_copy(std::begin(data4), std::end(data4), osit);
    std::cout << std::endl;

    std::array<int, 15> data5 = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    no_dupes_copy(std::begin(data5), std::end(data5), osit);
    std::cout << std::endl;

    std::forward_list<int> data6 = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    no_dupes_copy(std::begin(data6), std::end(data6), osit);
    std::cout << std::endl;

    std::initializer_list<int> data7 = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    no_dupes_copy(std::begin(data7), std::end(data7), osit);
    std::cout << std::endl;

    #else

    const std::size_t N = sizeof data1 / sizeof data1[0];

    std::vector<int> data2(&data1[0], &data1[N]);
    no_dupes_copy(data2.begin(), data2.end(), osit);
    std::cout << std::endl;

    std::list<int> data3(&data1[0], &data1[N]);
    no_dupes_copy(data3.begin(), data3.end(), osit);
    std::cout << std::endl;

    std::deque<int> data4(&data1[0], &data1[N]);
    no_dupes_copy(data4.end(), data4.end(), osit);
    std::cout << std::endl;

    #endif
}
