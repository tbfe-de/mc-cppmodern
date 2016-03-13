// http://coliru.stacked-crooked.com/a/503cb1be438af585

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
              
namespace my {
    class point {
    public:
        point(const point&)             =default;
        point(point&&)                  =default;
        point& operator=(const point&)  =default;
        point& operator=(point&&)       =default;
        point()                         =default;
    };
    point operator+(const point&, const point&) { return {}; }
}
              
void foo(my::point&)        {}
void foo(const my::point&)  {}
void foo(my::point&&)       {}
              
int main() {
    my::point pt1;
    const my::point pt2(pt1);
//  const my::point pt2{pt1};
    foo(pt1);
    foo(pt2);
    foo(pt1 + pt2);
}
