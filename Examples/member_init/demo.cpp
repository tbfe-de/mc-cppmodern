#include <iostream>
#include <string>

#define PX(...)\
    ((void)(std::cout << __FILE__ ":" << __LINE__\
                      << "\t" #__VA_ARGS__ " --> " << (__VA_ARGS__)\
                      << std::endl))

#if 1
#include <typeinfo>
#define PT(...)\
    ((void)(std::cout << __FILE__ ":" << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (typeid(__VA_ARGS__).name())\
                      << std::endl))
#else
#include <boost/type_index.hpp>
#define PT(...)\
    ((void)(std::cout << __FILE__ ":" << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << boost::typeindex::type_id_with_cvr<__VA_ARGS__>()\
                      << std::endl))
#endif

#include <cmath>

namespace my {
    class xy {
    public:
        // rectangular coordinate
        const double xr = 0.0;
        const double yr = 0.0;
        // polar coordinates
        const double rad = 0.0;
        const double deg = 0.0;
    private:
        xy(double x, double y, double r, double d)
            : xr(x), yr(y), rad(r), deg(d) {} 
    protected:
        xy() {}
    public:
        enum coord { rect_tag, pol_tag };
        template<coord> struct ctor_tag {};
        using rect = ctor_tag<rect_tag>;
        using pol = ctor_tag<pol_tag>;
        // selector for constructor
        xy(double x, double y, rect)
            : xr(x), yr(y), rad(std::sqrt(x*x + y*y)), deg(std::atan2(x, y))
        {}
        xy(double r, double d, pol)
            : xr(r*std::cos(d)), yr(r*std::sin(d)), rad(r), deg(d)
	{}
        std::string to_string() const {
            return "{xr=" + std::to_string(xr) + ", yr=" + std::to_string(yr)
               + " | rad=" + std::to_string(rad) + ", deg=" + std::to_string(deg)
               + "}";
        }
    };
        
    class point : private xy {
    public:
        using xy::xr;
        using xy::yr;
        using xy::rad;
        using xy::deg;
        using xy::rect;
        using xy::pol;
    #if 1
        point() : xy() {}
        point(double x, double y, rect) : xy(x, y, rect{}) {}
        point(double r, double d, pol) : xy(r, d, pol{}) {}
    #else
        using xy::xy;
    #endif
        std::string xy_to_string() const {return xy::to_string();}
    };
    std::ostream& operator<<(std::ostream& lhs, const point& rhs) {
        return lhs << ("point" + rhs.xy_to_string());
    }

    class distance : private xy {
    public:
        using xy::xr;
        using xy::yr;
        using xy::rad;
        using xy::deg;
        using xy::rect;
        using xy::pol;
    #if 1
        distance() : xy(0.0, 0.0, rect{}) {}
        distance(double x, double y, rect) : xy(x, y, rect{}) {}
        distance(double r, double d, pol) : xy(r, d, pol{}) {}
    #else
        using xy::xy;
    #endif
        std::string xy_to_string() const {return xy::to_string();}
    };
    std::ostream& operator<<(std::ostream& lhs, const distance& rhs) {
        return lhs << ("distance" + rhs.xy_to_string());
    }

    point operator+(point lhs, distance rhs) {
        return {lhs.xr + rhs.xr, lhs.yr + rhs.yr, xy::rect{}};
    }
    point operator+(distance lhs, point rhs) {
        return {lhs.xr + rhs.xr, lhs.yr + rhs.yr, xy::rect{}};
    }
    distance operator+(distance lhs, distance rhs) {
        return {lhs.xr + rhs.xr, lhs.yr + rhs.yr, xy::rect{}};
    }
    distance operator-(point lhs, point rhs) {
        return {lhs.xr - rhs.xr, lhs.yr - rhs.yr, xy::rect{}};
    }
    point operator*(double lhs, point rhs) {
        return {rhs.rad, lhs * rhs.deg, xy::pol{}};
    }
    point operator*(point lhs, double rhs) {
        return {lhs.rad, lhs.deg * rhs, xy::pol{}};
    }
    point operator/(point lhs, double rhs) {
        return {lhs.rad, lhs.deg / rhs, xy::pol{}};
    }
    distance operator*(double lhs, distance rhs) {
        return {lhs * rhs.rad, rhs.deg, xy::pol{}};
    }
    distance operator*(distance lhs, double rhs) {
        return {lhs.rad * rhs, lhs.deg, xy::pol{}};
    }
    distance operator/(distance lhs, double rhs) {
        return {lhs.rad / rhs, lhs.deg, xy::pol{}};
    }
}

void xy_points_and_distances() {
    const auto PI = 2*std::acos(double{});	PX(PI);
    using rect = my::xy::rect;
    using pol = my::xy::pol;
    my::xy a{3.0, 4.0, my::xy::rect{}};         PX(a.to_string());
    my::xy b{std::sqrt(2), PI/4, pol{}};        PX(b.to_string());
    my::point origin{};                         PX(origin);
    my::point p1{std::sqrt(3)/2, 0.5, rect{}};  PX(p1);
    my::point p2{1.0, PI, pol{}};               PX(p2);
    my::distance d1{-1.0, 0, pol{}};            PX(d1);
    my::distance d2{2*d1};                      PX(d2);
                                                PX(p1 - origin);
                                                PX(p1*(PI/3));
    PX(my::point{1.0, 1.0, rect{}}/2);
    PX(my::point{origin + 0*(p1 - origin)});
}

#include <sstream>
namespace my {
    struct clazz {
        std::ostream &os;
        ~clazz() {
            os << "clazz d'tor executed";
        }
        clazz(std::ostream& os_, bool b) try : os(os_) {
            if (b) throw "clazz(bool) c'tor body";
        } catch(const char *thrown_by) {
            os << "caught in clazz(bool) c'tor: " << thrown_by;
        };
        clazz(std::ostream &os_, int x) try : clazz(os_, x == 0) {
            if (x == 1) throw "clazz(int) c'tor body";
        } catch(const char *thrown_by) {
            os << "caught in clazz(int) c'tor: " << thrown_by;
        }
    };
}

void ctor_delegation_throw() {
    std::ostringstream os;
    try {
        auto arg = false;                     PX(arg);
    //  auto arg = true;                      PX(arg);
    //  auto arg = 0;                         PX(arg);
    //  auto arg = 1;                         PX(arg);
    //  auto arg = 42;                        PX(arg);
        PX(my::clazz(os, arg), os.str());     PX(os.str());
    } catch (const char *thrown_by) {
        PX("caught in test context", thrown_by);
    }
}

int main() {
    std::cout.setf(std::ios::boolalpha);
//  xy_points_and_distances();
    ctor_delegation_throw();
}

