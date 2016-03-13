// http://coliru.stacked-crooked.com/a/72b939757e46d642

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
              
#include <cmath>
#include <string>

namespace my {
    class point {
        double xc, yc;
    public:
        point(double xc_ = 0.0, double yc_ = 0.0)
            : xc(xc_), yc(yc_)
        {}
        point(const point&) =default;
        point& operator=(const point&) =default;
        std::string to_string() const {
            return "my::point(xc=" + std::to_string(xc)
                         + ", yc=" + std::to_string(yc)
                         + ")";
        }
    };
}

my::point foo(double z) {
    const double xy = std::sqrt(2.0)*z;
    return my::point{xy, xy};
}

int main() {
    my::point pt1{3.0, 4.0};    PX(pt1.to_string());
    my::point pt2{pt1};         PX(pt2.to_string());
    my::point pt3{foo(1.0)};    PX(pt3.to_string());
}
