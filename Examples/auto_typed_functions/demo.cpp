// http://coliru.stacked-crooked.com/a/0a4e2f27c3d8dbdb

#if __cplusplus < 201402L
#error "some C++14 features are mandatory to compile this!"
#endif

#include <iostream>

template<typename> struct typeprinter;

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
#define PT(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << typeprinter<__VA_ARGS__>::str() << std::endl;
              
#define GEN_typeprinter(t)\
    template<> struct typeprinter<t>\
    {static auto str() {return #t;}};\
    template<> struct typeprinter<unsigned t>\
    {static auto str() {return "unsigned " #t;} };
GEN_typeprinter(short)
GEN_typeprinter(int)
GEN_typeprinter(long)
GEN_typeprinter(long long)
#undef GEN_typeprinter
#define GEN_typeprinter(t)\
    template<> struct typeprinter<t>\
    { static const char *str() {return #t;} };
GEN_typeprinter(float)
GEN_typeprinter(double)
GEN_typeprinter(long double)

namespace my {
    template<std::size_t Octets>
    struct arithmetic {
        // ...
    };
    
    template<std::size_t N1, std::size_t N2>
    auto operator+(arithmetic<N1>, arithmetic<N2>) -> arithmetic<std::max(N1, N2)+1>;

    template<std::size_t N1, std::size_t N2>
    auto operator*(arithmetic<N1>, arithmetic<N2>) -> arithmetic<N1+N2>;    
}
#include <string>

template<std::size_t N>
struct typeprinter<my::arithmetic<N>>
{static auto str() {return "arithmetic<" + std::to_string(N) + ">";}};

#if 0

template<typename T1, typename T2>
auto add(T1&& lhs, T2&& rhs) -> decltype(lhs+rhs);
// decltype(*((typename std::remove_reference<T1>::type*)0)
//       + *((typename std::remove_reference<T2>::type*)0)) add(T1&&, T2&&);

template<typename T1, typename T2>
auto mul(T1&& lhs, T2&& rhs) -> decltype(lhs*rhs);

#else

template<typename T1, typename T2>
auto add(T1&& lhs, T2&& rhs) {
    return lhs+rhs;
}

template<typename T1, typename T2>
auto mul(T1&& lhs, T2&& rhs) {
    return lhs*rhs;
}

#endif

int main() {
    auto x = 6*7L;  PX(x)   PT(decltype(x))
                            PT(decltype(12u+x))
    my::arithmetic<3> a;    PT(decltype(a))
    my::arithmetic<6> b;    PT(decltype(b))
                            PT(decltype(a+b))
                            PT(decltype(a*b))
    
    PT(decltype(add(a, b)))
    PT(decltype(mul(a, b)))
    PT(decltype(mul(add(a, b), b)))
}
