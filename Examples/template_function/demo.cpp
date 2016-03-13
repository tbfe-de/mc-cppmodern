// http://coliru.stacked-crooked.com/a/08d2a5bf1ceffd78

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FILE__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))

#define PT(...)\
    ((void)(std::cout << __FILE__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str() << std::endl))
#include <string>
template<typename T> struct typeprinter;
template<> struct typeprinter<bool> {static std::string str() {return "bool";}};
template<> struct typeprinter<short> {static std::string str() {return "short";}};
template<> struct typeprinter<int> {static std::string str() {return "int";}};
template<> struct typeprinter<double> {static std::string str() {return "double";}};

namespace my {
#if 0
    template<typename T>
    T square(const T& arg) { return arg*arg; }
#else
    template<typename T>
    auto square(const T& arg) -> decltype(arg*arg) {
        return arg*arg;
    }
#endif

#if 0
    auto square(bool arg) -> bool {
        return arg;
    }
#endif
}

void square_template() {
    short a = 4;            PT(decltype(a)); PX(a);
    PX(my::square(a));      PT(decltype(my::square(a)));

    int b = 10;             PT(decltype(b)); PX(b);
    PX(my::square(b));      PT(decltype(my::square(b)));

    double c = 1.4142;      PT(decltype(c)); PX(c);
    PX(my::square(c));      PT(decltype(my::square(c)));

    PX(my::square(2));      PX(my::square<double>(2));
    PX(my::square(2.5));    PX(my::square<int>(2.5));

    PX(my::square(false));  PT(decltype(my::square(false)));
    PX(my::square(true));   PT(decltype(my::square(true)));

    PX(my::square<double>(true));
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    std::cout.setf(std::ios::fixed);
    std::cout.precision(3);
    square_template();
}
