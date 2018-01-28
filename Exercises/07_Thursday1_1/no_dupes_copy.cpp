#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

template<typename InIt, typename OutIt>
OutIt no_dupes_copy(InIt b, InIt e, OutIt t) {
    using ElType = typename std::iterator_traits<InIt>::value_type;
    std::set<ElType> s;
    while (b != e) {
        const auto p = s.insert(*b++);
        if (p.second)
            *t++ = * p.first;
    }
    return t;
}

int main() {
    int data[] = { 3, 1, 3, 7, 7, 2, 1, 3, 4, 4, 4, 1, 7, 1, 9 };
    std::ostream_iterator<int> osit(std::cout, "; ");

    no_dupes_copy(std::begin(data), std::end(data), osit);
    std::cout << std::endl;

    std::unique_copy(std::begin(data), std::end(data), osit);
    std::cout << std::endl;
}
