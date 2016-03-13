// http://coliru.stacked-crooked.com/a/4dcb87353c1e1a84

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;

#include <cmath>

namespace my {
    class point {
        double xc, yc;
    #if 0
        double radius, angle;
        bool mod{true};
        void sync_ra() const {
            point &t = *const_cast<point*>(this);
            t.radius = std::sqrt(xc*xc + yc*yc);
            t.angle = std::atan2(xc, yc);
            t.mod = false;
        }
    #else
        mutable double radius, angle;
        mutable bool mod{true};
        void sync_ra() const {
            radius = std::sqrt(xc*xc + yc*yc);
            angle = std::atan2(xc, yc);
            mod = false;
        }
    #endif
    public:
        point(double xc_, double yc_) : xc(xc_), yc(yc_) {}
        double x() const { return xc; }
        double y() const { return yc; }
        double r() const { if (mod) sync_ra(); return radius; }
        double a() const { if (mod) sync_ra(); return angle; }
        void x(double xc_) { xc = xc_; mod = true; }
        void y(double yc_) { yc = yc_; mod = true; }
        point &shift_x(double xd) { xc += xd; mod = true; return *this; }
        point &shift_y(double yd) { yc += yd; mod = true; return *this; }
        point &rotate(double ad) {
            sync_ra();
            angle += ad;
            xc = radius*std::cos(angle);
            yc = radius*std::sin(angle);
            return *this;
        }
    };
    std::ostream& operator<<(std::ostream& os, const point& pt) {
        return os << "my::point{" << pt.x() << ", " << pt.y() << "}";
    }
    const/*expr*/ double PI = 2*acos(0.0);
    double deg2rad(double d) { return PI*d/180.0; }
    double rad2deg(double r) { return 180.0*r/PI; }
}

std::string polar(const my::point &pt) {
    return "radius=" + std::to_string(pt.r())
        + ", angle=" + std::to_string(pt.a());
}

void shiftxy(my::point &pt, double xd, double yd) {
    pt.shift_x(xd).shift_y(yd);
}

int main() {
    my::point pt{3.0, 4.0};         
    PX(pt)
//  PX(pt.x()) PX(pt.y()) PX(pt.r()) PX(pt.a())
    PX(my::rad2deg(pt.a()))
    PX(pt.shift_x(2.0).shift_y(1.0))
    PX(polar(pt))
    PX(my::rad2deg(pt.a()))
    PX(pt.rotate(my::deg2rad(45)))
    PX(2*pt.a())
    PX(pt.rotate(my::deg2rad(180)))
    PX(shiftxy(pt, 10/std::sqrt(2), 0), pt)
}
