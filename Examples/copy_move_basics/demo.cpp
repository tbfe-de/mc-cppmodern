// http://coliru.stacked-crooked.com/a/926b040c36d2a19d

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
              
namespace my {
    class point {
    #if 0
        point(const point&);            // copy contructor
        point(point&&);                 // move constructor
        point& operator=(const point&); // copy assignment
        point& operator=(point&&);      // move assignment
    public:
        point();                        // default constructor
    #endif
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
