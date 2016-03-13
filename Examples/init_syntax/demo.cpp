// http://coliru.stacked-crooked.com/a/08a8e43822fdc323

#include <iostream>
#include <functional>
    
#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> " << (__VA_ARGS__) << std::endl
    
#define PT(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << typeprinter<__VA_ARGS__>::str() << std::endl
              
template<typename> struct typeprinter;

#define GEN_typeprinter(t)\
    template<> struct typeprinter<t>\
    {static auto str() -> const char* {return #t;}};\
    template<> struct typeprinter<unsigned t>\
    {static auto str() -> const char* {return "unsigned " #t;} };
GEN_typeprinter(short)
GEN_typeprinter(int)
GEN_typeprinter(long)
GEN_typeprinter(long long)

#include <string>

template<> struct typeprinter<std::string()>
{static auto str() -> const char* {return "std::string()";}};
    
namespace my {
    struct point {
        double x, y;
    };
}

int main() {
#if 1
    // classic initialisation
    int x = 42;                     PX(x);
    std::string hello("hi!");       PX(hello);
//  std::string empty();            PT(decltype(empty)); 
    std::string empty;              PX(empty);
    my::point pt = {3.0, 4.0};      PX(pt.x); PX(pt.y);
    int zero = int();               PX(zero);
    unsigned long mask = 0xFF;      PX(mask); PT(decltype(mask));
#else
    int x{42};                      PX(x);
    std::string hello{"hi!"};       PX(hello);
    std::string empty{};            PX(empty);
    my::point pt{3.0, 4.0};         PX(pt.x); PX(pt.y);
    int zero{};                     PX(zero);
    auto mask = 0xFF;               PX(mask); PT(decltype(mask));
//  auto mask= unsigned long{0xFF}; PX(mask); PT(decltype(mask));
#endif
}
