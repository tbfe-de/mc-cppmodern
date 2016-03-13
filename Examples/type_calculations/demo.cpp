// http://coliru.stacked-crooked.com/a/8e1d4212337eb36b

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ << " --> "\
                      << (__VA_ARGS__) << std::endl))
#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ << " --> "\
                      << typeprinter<__VA_ARGS__>::str() << std::endl))

template<typename> struct typeprinter;

template<> struct typeprinter<short> {static std::string str() {return "short";}};
template<> struct typeprinter<int> {static std::string str() {return "int";}};
template<> struct typeprinter<long> {static std::string str() {return "long";}};
template<> struct typeprinter<long long> {static std::string str() {return "long long";}};
template<> struct typeprinter<float> {static std::string str() {return "float";}};
template<> struct typeprinter<double> {static std::string str() {return "double";}};
template<> struct typeprinter<long double> {static std::string str() {return "long double";}};

template<typename T> struct typeprinter<T&>
{static std::string str() {return typeprinter<T>::str() + "&";}};
template<typename T> struct typeprinter<T&&>
{static std::string str() {return typeprinter<T>::str() + "&&";}};
template<typename T> struct typeprinter<const T>
{static std::string str() {return "const " + typeprinter<T>::str();}};

#include <type_traits>

namespace my {
#if 0
    template<typename T>
    struct argument_type {using type = const T&;};

    template<> struct argument_type<short> {using type = short;};
    template<> struct argument_type<int> {using type = int;};
    template<> struct argument_type<float> {using type = float;};
#else
    #if 1
    template<typename T>
    struct argument_type {
        constexpr static auto fits_in_register = (sizeof(T) <= 4);
        #if 1
        using type = typename
                     std::conditional<fits_in_register,
                                      T, const T&>::type;
        #else
        using type = std::conditional_t<fits_in_register,
                                        T, const T&>;
        #endif
    };
    #else
    template<typename T>
    struct argument_type
        : std::conditional_t<(sizeof(T) <= 4), T, const T&> {};
    #endif
#endif

    template<typename T>
    struct fixpoint {
        const T x{};
        const T y{};
        using argtype = typename argument_type<T>::type;
        fixpoint(argtype x_, argtype y_)
            : x(x_), y(y_) {
            PT(argtype);
        }
        fixpoint shifted(argtype xd, argtype yd) {
            PT(argtype);
            return {x + xd, y + yd};
        }
    };
}

void test_argument_type() {
    PT(my::argument_type<short>::type);
    PT(my::argument_type<int>::type);
    PT(my::argument_type<long>::type);
    PT(my::argument_type<long long>::type);
    PT(my::argument_type<float>::type);
    PT(my::argument_type<double>::type);
    PT(my::argument_type<long double>::type);

//  auto pt = my::fixpoint<double>{3.0, 4.0}; PX(pt.x); PX(pt.y);
    auto pt = my::fixpoint<float>{3.0, 4.0};  PX(pt.x); PX(pt.y);
    auto pt2 = pt.shifted(1, -2);             PX(pt2.x); PX(pt2.y);
}

#include <iterator>
#include <set>
#include <utility>

namespace my {
    template<typename InIt, typename OutIt>
    OutIt noduplicates(InIt from, InIt upto, OutIt dest) {
        using element_type =
               typename std::iterator_traits<InIt>::value_type;
            // typename InIt::value_type;
        std::set<element_type> seen;
        while (from != upto) {
        //  if (std::get<bool>(seen.insert(*from)))
            if (seen.insert(*from).second)
    	    *dest++ = *from;
            ++from;
        }
        return dest;
    }
}

#include <algorithm>
#include <sstream>
#include <vector>

void test_noduplicates() {
    auto test_data              = {1, 4, 12, 44, 3, 2, 24, 4, 4, 23, 12, 2, 3};
//  int test_data[]             = {1, 4, 12, 44, 3, 2, 24, 4, 4, 23, 12, 2, 3};
//  std::vector<int> test_data  = {1, 4, 12, 44, 3, 2, 24, 4, 4, 23, 12, 2, 3};
    std::ostringstream test_noduplicates;
    my::noduplicates(std::begin(test_data), std::end(test_data),
                     std::ostream_iterator<int>(test_noduplicates, " "));
    PX(test_noduplicates.str());
    std::ostringstream test_unique_copy;
    std::unique_copy(std::begin(test_data), std::end(test_data),
                     std::ostream_iterator<int>(test_unique_copy, " "));
    PX(test_unique_copy.str());
 }

int main() {
    test_argument_type();
    test_noduplicates();
}
