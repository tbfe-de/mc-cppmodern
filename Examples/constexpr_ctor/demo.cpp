
#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;

#include <cmath>

namespace my {
    struct point {
        const double x, y;
        constexpr point(double x_, double y_) : x(x_), y(y_) {}
    };
    constexpr double square(double v) {
        return v*v;
    }
    std::ostream& operator<<(std::ostream& lhs, const point& rhs) {
        return lhs << "my::point{" << rhs.x << ", " << rhs.y << "}";
    }
    /*constexpr*/ double operator-(const point lhs, const point rhs) {
        return std::sqrt(square(lhs.x-rhs.x) + square(lhs.y-rhs.y));
    }
}

int main() {
    constexpr my::point a{0, 3};        PX(a)
    constexpr my::point b{4, 0};        PX(b)
    const/*expr*/ double dist{a-b};     PX(dist)
}
