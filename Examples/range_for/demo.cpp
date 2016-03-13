// https://coliru.stacked-crooked.com/a/94bcea8326eb854b

#include <iostream>

#define PX(...)\
    std::cout << __FILE__ ":" << __LINE__\
              << "\t" #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;

#include <string>
#include <sstream>

#if 0
std::string native_array() {
    const int primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    };
    std::ostringstream os;
    for (auto e : primes)
        os << e << ' ';
    return os.str();
}
#else
#include <type_traits>
template<typename T, std::size_t N>
typename std::enable_if<!std::is_same<T, char>::value,
std::ostream&>::type operator<<(std::ostream& lhs, const T (&rhs)[N]) {
    for (auto e : rhs)
        lhs << e << ' ';
    return lhs;
}
std::string native_array() {
    const int primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    };
    std::ostringstream os;
    os << primes;
    return os.str();
}
#endif

#include <array>
#if 0
std::string std_array() {
    std::array<int, 10> primes = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    };
    std::ostringstream os;
    for (auto e : primes)
        os << e << ' ';
    return os.str();
}
#else
template<typename T, std::size_t N>
std::ostream& operator<<(std::ostream& lhs, const std::array<T, N> &rhs) {
    for (auto e : rhs)
        lhs << e << ' ';
    return lhs;
}
std::string std_array() {
    std::ostringstream os;
    std::array<int, 10> primes = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    };
    os << primes;
    return os.str();
}
#endif

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
std::string seq_container() {
//  using my_container = std::vector<int>;
//  using my_container = std::deque<int>;
//  using my_container = std::list<int>;
    using my_container = std::forward_list<int>;
    my_container primes{{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}};
    std::ostringstream os;
    for (auto e : primes)
        os << e << ' ';
    return os.str();
}

#include <set>
#include <unordered_set>
std::string set_container() {
    using my_container = std::set<int>;
//  using my_container = std::unordered_set<int>;
    my_container primes{{2, 3, 5, 7, 11, 13, 17, 19, 23, 29}};
    std::ostringstream os;
    for (auto e : primes)
        os << e << ' ';
    return os.str();
}

#include <map>
#include <unordered_map>
std::string map_container() {
    using my_container = std::map<int, bool>;
//  using my_container = std::unordered_map<int, bool>;
    my_container is_prime{{
        {1, false /* primes.utm.edu/notes/faq/one.html */ },
        {2, true},      {3, true},      {4, false},     {5, true},
        {6, false},     {7, true},      {8, false},     {9, false},
        {10, false},    {11, true},     {12, false},    {13, true},
        {14, false},    {15, false},    {16, false},    {17, true},
        {18, false},    {19, true},     {20, false},    {21, false},
        {22, false},    {23, true},     {24, false},    {25, false},
        {26, false},    {27, false},    {28, false},    {29, true},
    }};
    std::ostringstream os;
    for (auto e : is_prime) {
        if (e.second)
            os << e.first << ' ';
    }
    return os.str();
}

std::string init_list_container() {
    std::ostringstream os;
    for (auto e : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29})
        os << e << ' ';
    return os.str();
}

namespace my {
    class primes {
        const std::size_t count;
    public:
        primes(int c) : count(c) {}
        class iterator;
        iterator begin();
        iterator end();
    };
    class primes::iterator {
        mutable std::vector<int> known = {3};
        std::size_t current;
        iterator(std::size_t n) : current(n) {}
        friend class primes;
    public:
        int operator*() const {
            if (current == 0) return 2;
            int candidate = known.back() + 2;
            while (known.size() <= current-1) {
                auto found = true;
            // RESTART:
                for (auto p : known) {
                    if (candidate % p == 0) {
                        candidate += 2;
                        // goto RESTART;
                        found = false;
                        break;
                    }
                }
                if (found)
                    known.push_back(candidate);
            }
            return known.at(current-1);
        }
        iterator& operator++() {
            ++current; return *this;
        }
        iterator operator++(int) {
            return iterator{current+1};
        }
        friend bool operator==(iterator lhs, iterator rhs) {
            return lhs.current == rhs.current;
        }
        friend bool operator!=(iterator lhs, iterator rhs) {
            return lhs.current != rhs.current;
        }
    };
    auto primes::begin() -> iterator { return {0}; }
    auto primes::end() -> iterator { return {count}; }
}

std::string primes_container() {
    my::primes primes(10);
    std::ostringstream os;
    for (auto e : primes)
        os << e << ' ';
    return os.str();
}

namespace my {
    enum class colors { R, G, B, _end };
    const char *to_string(colors c) {
        switch (c) {
            case colors::R: return "Red";
            case colors::G: return "Green";
            case colors::B: return "Blue";
            default: return "??color??";
        }
    }
    std::ostream& operator<<(std::ostream& lhs, colors rhs) {
        return lhs << to_string(rhs);
    }
    auto all_colors = { colors::R, colors::G, colors::B };
    
    class colors_iterator {
        colors c;
    public:
        explicit colors_iterator(colors c_ = colors::_end) : c(c_) {}
        colors operator*() const { return c; }
        colors_iterator operator++() {
            c = static_cast<colors>(static_cast<int>(c)+1);
            return *this;
        }
        colors_iterator operator++(int) {
            return colors_iterator{static_cast<colors>(static_cast<int>(c)+1)};
        }
    };
    bool operator==(colors_iterator lhs, colors_iterator rhs) { return *lhs == *rhs; }
    bool operator!=(colors_iterator lhs, colors_iterator rhs) { return *lhs != *rhs; }
#if 0
    bool operator<(colors_iterator lhs, colors_iterator rhs) { return *lhs < *rhs; }
    bool operator<=(colors_iterator lhs, colors_iterator rhs) { return *lhs <= *rhs; }
    bool operator>=(colors_iterator lhs, colors_iterator rhs) { return *lhs >= *rhs; }
    bool operator>(colors_iterator lhs, colors_iterator rhs) { return *lhs > *rhs; }
#endif
    colors_iterator begin(colors) { return colors_iterator{colors::R}; }
    colors_iterator end(colors) { return colors_iterator{}; }
}

std::string colors_container() {
    std::ostringstream os;
#if 1
    for (auto e : my::colors{})
        os << e << ' ';
#else
    for (auto e : my::all_colors)
        os << e << ' ';
#endif
    return os.str();
}

int main()
{
    PX(native_array());
    PX(std_array());
    PX(seq_container());
    PX(set_container());
    PX(map_container());
    PX(primes_container());
    PX(colors_container());
}
