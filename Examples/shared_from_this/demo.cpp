// http://coliru.stacked-crooked.com/a/47eabec6a5458bfb

#include <iostream>
#include <string>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ":" << __LINE__\
                      << "\t"  #__VA_ARGS__ " --> " << (__VA_ARGS__)\
                      << std::endl))
                      
#include <memory>

namespace my {
    std::ostream &operator<<(std::ostream &lhs, const struct clazz*);
    
    struct clazz : public std::enable_shared_from_this<clazz> {
        int x;
        int get_x() const {return x;}
        int set_x(int x_) {return (x = x_);}
        std::shared_ptr<clazz> BAD_get_self() {
                return std::shared_ptr<clazz>(this);
        }
    #if 0
        std::shared_ptr<clazz> get_self() {return shared_from_this();}
    #else
        using std::enable_shared_from_this<clazz>::shared_from_this;
    #endif
    #define P(t)\
        std::cout << "\t=> my::clazz " t << this << std::endl;
        clazz()                 {P("default c'tor")}
        ~clazz()                {P("d'tor")}
    #if 0
        clazz(int x_) : x(x_)   {P("(int) c'tor")}
        clazz(const clazz&)     {P("copy c'tor")}
        clazz(clazz&&)          {P("move c'tor")}
        clazz& operator=(const clazz&) =delete;
        clazz& operator=(clazz&&) =delete;
        clazz(bool b)           {P("(bool) c'tor");
                                 if (b) throw "c'tor failed";}
    #endif
    #undef P
    };

    std::ostream &operator<<(std::ostream &lhs, const clazz *rhs) {
    //  return lhs << '@' << (rhs ? (void*)rhs : "@nullptr");
        if (rhs)
            return lhs << '@' << (void*)rhs;
        else
            return lhs << "@nullptr";
    }
}

void bad_shared_from_this() {
    auto ptz = std::make_shared<my::clazz>();
    PX(ptz.get()); PX(ptz.use_count()); PX(ptz.unique());
    
    auto pts = ptz->BAD_get_self();
    PX(pts.get()); PX(pts.use_count()); PX(pts.unique());
}

void shared_from_this() {
    auto ptz = std::make_shared<my::clazz>();
    PX(ptz.get()); PX(ptz.use_count()); PX(ptz.unique());
    
//  auto pts = ptz->get_self();
    auto pts = ptz->shared_from_this();
    PX(pts.get()); PX(pts.use_count()); PX(pts.unique());
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    bad_shared_from_this();
//  shared_from_this();
}
