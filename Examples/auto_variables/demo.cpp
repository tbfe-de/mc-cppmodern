// http://coliru.stacked-crooked.com/a/eaef38306c4db828

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
    { static const char *str() {return #t;} };
GEN_typeprinter(char)
GEN_typeprinter(int)
GEN_typeprinter(const int)
GEN_typeprinter(int&)
GEN_typeprinter(const int&)
GEN_typeprinter(int&&)
GEN_typeprinter(int*)
GEN_typeprinter(const int*)
GEN_typeprinter(int *const)
GEN_typeprinter(const int *const)

int main() {
    auto a = 42;            PT(decltype(a))
    auto b = a;             PT(decltype(b))
    const int c = 2*a;      PT(decltype(c))
    auto d = c;             PT(decltype(d))
    auto& e = d;            PT(decltype(e))
    const auto& f = e;      PT(decltype(f))
    auto& g = c;            PT(decltype(g))
    auto h = &a;            PT(decltype(h))
    auto i = &c;            PT(decltype(h))
    auto* j = &a;           PT(decltype(h))
//  auto* k= a;             PT(decltype(k))
    auto z = 'z' /*+1*/;    PT(decltype(z))
}
