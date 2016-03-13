// http://coliru.stacked-crooked.com/a/ec215758bf9e53f3

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
              
namespace my {
    class point {
    public:
        const char* ctor;
        point()                         : ctor("default c'tor") {}
        point(const point&)             : ctor("copy c'tor") {}
        point(point&&)                  : ctor("move c'tor") {}
//      point& operator=(const point&)  {ctor = "copy assign"; return *this;}
//      point& operator=(point&&)       {ctor = "move assign"; return *this;}
    };
    point operator+(const point&, const point&) { return {}; }
}

template<> struct typeprinter<my::point>
{ static const char *str() {return "my::point";} };
template<> struct typeprinter<my::point&>
{ static const char *str() {return "my::point&";} };
template<> struct typeprinter<const my::point&>
{ static const char *str() {return "const my::point&";} };
template<> struct typeprinter<my::point&&>
{ static const char *str() {return "my::point&&";} };
         
template<typename T>
void foo(T&& arg, bool b = false) {
    PT(T)
    PT(decltype(arg))
    PT(decltype(std::forward<T>(arg)))
    T loc(std::forward<T>(arg));    
//  T loc{std::forward<T>(arg)};    
    PX(&loc) if (b) PX(loc.ctor)
}

int main() {
    my::point pt1;                      PX(&pt1) PX(pt1.ctor)
    foo(pt1);
    const my::point pt2(pt1);           PX(&pt2) PX(pt2.ctor)
//  const my::point pt2{pt1};           PX(&pt2) PX(pt2.ctor)
    foo(pt2);
    my::point pt3(std::move(pt1));      PX(&pt3) PX(pt3.ctor)
//  my::point pt3{std::move(pt1)};      PX(&pt3) PX(pt3.ctor)
    foo(pt1 + pt2, true);
}
