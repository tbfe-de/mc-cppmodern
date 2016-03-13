// http://coliru.stacked-crooked.com/a/2b9ac3df84185215

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
              
#include <cmath>
#include <cstring>

namespace my {
    class point {
        const char *nm;
        double xc, yc;
    public:
        point(const char *name, double xc_ = 0.0, double yc_ = 0.0)
            : nm(std::strcpy(new char[std::strlen(name)+1], name))
            , xc(xc_), yc(yc_)
        {}
        ~point() {
            delete[] nm;
        }
        point(const point&) =delete;
        point& operator=(const point&) =delete;
        point(point&& init) noexcept
            : nm(init.nm), xc(init.xc), yc(init.yc) {
            init.nm = nullptr;
        }
        point& operator=(point&& rhs) {
            if (this != &rhs) {
                delete[] nm;
                nm = rhs.nm;
                rhs.nm = nullptr;
                xc = rhs.xc;
                yc = rhs.yc;
            }
            return *this;
        }
        const char *name() const { return nm; }
        double x() const { return xc; }
        double y() const { return yc; }
        void x(double xc_) { xc = xc_; }
        void y(double yc_) { yc = yc_; }
    };
    std::string to_string(const point& rhs) {
        return "my::point(name=" + std::string(rhs.name())
                       + ", x=" + std::to_string(rhs.x())
                       + ", y=" + std::to_string(rhs.y())
                       + ")";
    }
    
    point make_point_rect(const char *name, double xc, double yc) {
        return point{name, xc, yc};
    }
    
    point make_point_polar(const char *name, double radius, double angle) {
        return point{name, radius*std::cos(angle), radius*std::sin(angle)};
    }
}

int main() {
    my::point pt1{"first", 3.0, 4.0};
    PX(to_string(pt1));
    my::point pt2{my::make_point_rect("other", 2.0, 2.0)};
    PX(to_string(pt2));
    pt1 = my::make_point_polar("second", 10.0, 0.0);
    PX(to_string(pt1));
    pt1.y(10);
//  pt2 = pt1;
    pt2 = std::move(pt1);
    PX(to_string(pt2));
    
}
