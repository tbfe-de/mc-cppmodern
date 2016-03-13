// http://coliru.stacked-crooked.com/a/cb9ba4b36734e074

#include <iostream>
#include <string>
#include <typeinfo>
#include <array>

#define PX(...)\
    (void)(std::cout << __FILE__ ":" << __LINE__\
                     << "\t" #__VA_ARGS__ " --> "\
                     << (__VA_ARGS__) << std::endl)
#if 1       
template<typename T> struct typeprinter
{ static std::string str() {return typeid(T).name(); } };

template<> struct typeprinter<void>
{ static std::string str() {return "void";} };
template<> struct typeprinter<int>
{ static std::string str() {return "int";} };
template<typename T> struct typeprinter<T*>
{ static std::string str() {return typeprinter<T>::str() + "*";} };
template<typename T> struct typeprinter<T&>
{ static std::string str() {return typeprinter<T>::str() + "&";} };
template<typename T> struct typeprinter<const T>
{ static std::string str() {return "const " + typeprinter<T>::str();} };
template<typename T, std::size_t N> struct typeprinter<T[N]>
{ static std::string str() {return typeprinter<T>::str() + "[" + std::to_string(N) + "]";} };
template<typename T, std::size_t N> struct typeprinter<std::array<T, N>>
{ static std::string str() {return "std::array<" + typeprinter<T>::str() + ", " + std::to_string(N) + ">";} };

#define PT(...)\
    (void)(std::cout << __FILE__ ":" << __LINE__\
                     << "\t" #__VA_ARGS__ " --> "\
                     << typeprinter<__VA_ARGS__>::str() << std::endl)
#else
#include <boost/type_index.hpp>
#define PT(...)\
    (void)(std::cout << __FILE__ ":" << __LINE__\
                     << "\t" #__VA_ARGS__ " --> "\
                     << boost::typeindex::type_id_with_cvr<__VA_ARGS__>()\
                     << std::endl)
#endif

#include <functional>
#include <stdexcept>

auto is_odd(int n) -> bool {return n%2; }

auto is_even(int n) -> bool {return !(n%2); }

struct check_odd {
    auto operator()(int n) -> bool {return (n%2 != 0);}
};

struct check_even {
    auto operator()(int n) -> bool {return (n%2 == 0);}
};

void function_basics() {
    PT(decltype(is_odd));
    PT(decltype(is_odd(3)) );   PX(is_odd(3));
    PT(decltype(is_even)   );
    PT(decltype(is_even(3)));   PX(is_even(3));
    PT(         check_odd  );   PX(check_odd{}(3));
    PT(         check_even );   PX(check_even{}(3));

    check_odd odd;      PT(decltype(odd));
    check_odd other;    PT(decltype(other));

    auto f1 = [](int n) {return n%2;};
                        PT(decltype(f1));
    auto f2 = [](int n) {return !(n%2);};
                        PT(decltype(f2));

    std::function<bool(int)> f;
                        PT(decltype(f));
                        PX((bool)f);
                     // PX(      f);
    f = f1;             PX((bool)f);
                        PX(f(42));
    f = f2;             PX((bool)f);
                        PX(f(42));
    f = nullptr;        PX((bool)f);
    try { f(42); }
    catch (std::exception &ex) {
                        PX(ex.what());
    }

    PT(std::function<double(double, int, bool)>);
    PT(std::function<double(double d, int i, bool b)>);
    PT(std::function<auto (double, int, bool) -> double>);
    PT(std::function<void()>);
#if 0
    PT(std::function<std::function<void(void)>(std::function<void(void)>)>);
#else
    using FVV = std::function<void(void)>;
    PT(std::function<FVV(FVV)>);
    PT(std::function<auto (FVV arg) -> FVV>);
#endif
}

void type_erasure() {
    std::function<bool(int)> f;
    PX((f = is_odd)                             (42));
    PX((f = is_even)                            (42));
    PX((f = check_odd{})                        (42));
    PX((f = check_even{})                       (42));
    PX((f = [](int n) -> bool {return n%2;})    (42));
    PX((f = [](int n) {return n%2;})            (42));
    PX((f = [](int) {return true;})             (42));
//  PX((f = [](int) {})                         (42));
//  PX((f = []() {return true;})                (42));
//  PX((f = [] {return true;})                  (42));
    PX((f = [](bool v) -> void* {return 0;})    (42));
}

void supported_operations() {
    std::function<bool(int)> f;         PT(decltype(f));

    std::function<int(short)> f2;       PT(decltype(f2));
    PT(decltype(f2 = f));               PT(decltype(f = f2));

    std::function<void(int)> f3{f2};    PT(decltype(f3 = f));
                                     // PT(decltype(f = f3));
                                     // PT(decltype(f3 = f2));

    PX(f ? "nullptr" : "not nullptr");
    PX((bool)(f = nullptr));
//  PX(      (f = nullptr));
    PX(f ? "nullptr" : "not nullptr");
    PX(f == nullptr); PX(nullptr == f);
    PX(f != nullptr); PX(nullptr != f);
//  PX(f == f);
}

#include <sstream>
#include <iterator>
#include <initializer_list>
#include <algorithm>

namespace my {
#if 1
    template<typename Pred>
    std::size_t count(const std::initializer_list<int>& values, Pred pred) {
    #if 0
        auto result = std::size_t{0};
        for (auto v : values) {
            if (pred(v))
                    ++result;
        }
        return result;
    #else
        return std::count_if(values.begin(), values.end(), pred);
    #endif
    }
#else
    std::size_t count(const std::initializer_list<int>& values,
                      std::function<bool(int)> pred) {
    #if 0
        auto result = std::size_t{0};
        for (auto v : values) {
            if (pred(v))
                    ++result;
        }
        return result;
    #else
        return std::count_if(values.begin(), values.end(), pred);
    #endif
    }
#endif
}

#include<sstream>

void function_vs_generic() {
    PX(my::count({2, 3, 12, 7, 5}, is_odd));
    PX(my::count({2, 3, 12, 7, 5}, check_odd{}));
    PX(my::count({2, 3, 12, 7, 5}, [](int n) {return (n%2 != 0);}));
    PX(my::count({2, 3, 12, 7, 5}, [](int n) -> bool {return n%2;}));
    PX(my::count({2, 3, 12, 7, 5}, [](int n) {return n%2;}));
    
    std::function<bool(int)> f = [](int n) {return n%2;};
//  std::function<void(int)> f = [](int) {};
//  std::function<bool()> f = []{return false;};
    PX(my::count({2, 3, 12, 7, 5}, f));
//  PX(my::count({2, 3, 12, 7, 5}, [](int) {}));
//  PX(my::count({2, 3, 12, 7, 5}, [] {return false;}));
}

long double powerof(long double value, int exponent) {
    long double result{1};
    while (exponent-- > 0)
        result *= value;
    return result;
}

void bind_basics() {
    using std::placeholders::_1;
    using std::placeholders::_2;
    PX(std::bind(powerof, _1, 2)        (4));
    PX(std::bind(powerof, _1, 3)        (4));
    PX(std::bind(powerof, 2, 3)         ());
    PX(std::bind(powerof, _1, _2)       (5, 2));
    PX(std::bind(powerof, _1, _2)       (5, 3));
    PX(std::bind(powerof, _2, _1)       (2, 5));
    PX(std::bind(powerof, _2, _1)       (3, 5));
}

namespace my {
    class xcopy {
        bool b;
    public: 
        xcopy() : b(false) {}
        bool toggle() {return (b = !b);}
        bool get() const {return b;}
     // assuming
        xcopy(const xcopy&)            =default;
        xcopy& operator=(const xcopy&) =default;
        xcopy(xcopy&&)                 =default;
        xcopy& operator=(xcopy&&)      =default;
    };
    class ncopy {
        bool b;
    public:
        ncopy() : b(false) {}
        bool toggle() {return (b = !b);}
        operator bool() const {return b;}
     // assuming
        ncopy(const ncopy&)            =delete;
        ncopy& operator=(const ncopy&) =delete;
        ncopy(ncopy&&)                 =default;
        ncopy& operator=(ncopy&&)      =default;
    };
}

void ref_and_cref() {
 
    my::xcopy cp;  PX(cp.get());

    PX((std::bind(&my::xcopy::toggle,  std::ref(cp)))   ());
    PX((std::bind(&my::xcopy::toggle,  std::ref(cp)))   ());
    PX((std::bind(&my::xcopy::toggle,           cp ))   ());
    PX((std::bind(&my::xcopy::toggle,           cp ))   ());
//  PX((std::bind(&my::xcopy::toggle, std::cref(cp)))   ());
    PX((std::bind(&my::xcopy::get,    std::cref(cp)))   ());
    PX((std::bind(&my::xcopy::get,     std::ref(cp)))   ());
    PX((std::bind(&my::xcopy::get,              cp ))   ());

    my::ncopy mv;  PX(mv);

    PX((std::bind(&my::ncopy::toggle,  std::ref(mv)))   ());
    PX((std::bind(&my::ncopy::toggle,  std::ref(mv)))   ());
//  PX((std::bind(&my::ncopy::toggle,           mv ))   ());
//  PX((std::bind(&my::ncopy::toggle,           mv ))   ());
//  PX((std::bind(&my::ncopy::toggle, std::cref(mv)))   ());
    PX((std::bind(&my::ncopy::operator bool, std::cref(mv))) ());
    PX((std::bind(&my::ncopy::operator bool,  std::ref(mv))) ());
//  PX((std::bind(&my::ncopy::operator bool,           mv )) ());

    const auto& ccp = cp;  PX(ccp.get());

//  PX((std::bind(&my::xcopy::toggle,  std::ref(ccp))) ());
//  PX((std::bind(&my::xcopy::toggle, std::cref(ccp))) ());
    PX((std::bind(&my::xcopy::get,     std::ref(ccp))) ());
    PX((std::bind(&my::xcopy::get,    std::cref(ccp))) ());
    PX((std::bind(&my::xcopy::get,              ccp )) ());

    const auto& cmv = std::move(mv);  PX(cmv);

//  PX((std::bind(&my::ncopy::toggle,         std::ref(cmv)))  ());
//  PX((std::bind(&my::ncopy::toggle,        std::cref(cmv)))  ());
    PX((std::bind(&my::ncopy::operator bool, std::cref(cmv)))  ());
    PX((std::bind(&my::ncopy::operator bool,  std::ref(cmv)))  ());
//  PX((std::bind(&my::ncopy::operator bool,           cmv ))  ());
}

#include <vector>
#include <sstream>

namespace my {
    struct xy {
        double x, y;
        std::string to_string() const {
            return "x=" + std::to_string(x)
               + ", y=" + std::to_string(y);
        }
    };
    class polygon {
        const std::vector<xy> points;
    public:
        polygon(std::initializer_list<xy> pts)
            : points(pts.begin(), pts.end()) {
        }
//      using pen_t = std::function<void(double x, double y)>;
        using pen_t = std::function<auto (double x, double y) -> void>;
        void draw(pen_t pen) const {
            if (points.empty()) return;
            for (const auto& pt : points)
                pen(pt.y, pt.x);
            pen(points.front().y, points.front().x);
        }
    };
    std::ostream &testpen(std::ostream &lhs, double x, double y) {
        return lhs << "<x:" << x << "|y:" << y << ">";
    }
    void relativepen(std::ostream &lhs, double xd, double yd) {
        lhs << "<xd:" << xd << "~yd:" << yd << ">";
    }
}

void function_callback() {
    std::ostringstream os;
    using namespace std::placeholders;
    const my::xy a{1.5, 7.0};   PX(a.to_string());
    const my::xy b{2.0, 3.0};   PX(b.to_string());
    const my::xy c{9.1, 2.5};   PX(c.to_string());
#if 0
    using std::bind;
    using std::ref;
    PX(my::polygon{a,b,c}.draw(bind(my::testpen, ref(os), _1, _2)), os.str());
//  PX(my::polygon{a,b,c}.draw(bind(my::testpen, ref(os), _2, _1)), os.str());
#else
#if 0
    my::polygon::pen_t adapted_pen = std::bind(my::testpen, std::ref(os), _2, _1);
//  const auto adapted_pen = std::bind(my::testpen, std::ref(os), _2, _1);
//  const auto adapted_pen = [&os](int y, int x) {my::testpen(os, x, y);};
#else
    #if 0
    const auto adapted_pen =
       [&os, xc=0.0, yc=0.0](double y, double x) mutable {
           my::relativepen(os, x-xc, y-yc); xc = x; yc = y;
       };
    #else
    my::xy pp{0,0}; // current pen position
    const auto adapted_pen =
       [&os, &pp](double y, double x) {
           my::relativepen(os, x-pp.x, y-pp.y); pp = {x, y};
       };
    #endif
#endif
    PT(decltype(adapted_pen));
    const auto triangle = my::polygon({a, b, c});
    PX(triangle.draw(adapted_pen), os.str());
#endif
    
}

#include <algorithm>
#include <sstream>

namespace my {
    class clazz {
        const char *nm;
    public:
        clazz(const char *n) : nm(n) {}
    //  clazz(const clazz&) =delete;
        clazz& operator=(const clazz&) =delete;
        void a(int v) const {os << nm <<  ".a(" << v << ") "; }
        void b(int v) const {os << nm <<  ".b(" << v << ") "; }
        void c(int v) const {os << nm <<  ".c(" << v << ") "; }
        void d(int v) const {os << nm <<  ".d(" << v << ") "; }
        static std::ostringstream os;
        static std::string os_str() {
            const auto result = os.str();
            os.str(std::string{});
            return result;
        }
    };
    std::ostringstream clazz::os;
}


std::string b_for_each_arg(const my::clazz& obj, std::initializer_list<int> arg_list) {
    using std::placeholders::_1;
    std::for_each(arg_list.begin(), arg_list.end(),
                  std::bind(&my::clazz::b, std::ref(obj), _1)
               // [&obj](int v) { obj.b(v); }
                 );
    return my::clazz::os_str();
}

std::string b_for_each_obj(std::initializer_list<my::clazz> obj_list, int arg) {
    using std::placeholders::_1;
    std::for_each(obj_list.begin(), obj_list.end(),
                  std::bind(&my::clazz::b, _1, arg)
               // [arg](const my::clazz& obj) { obj.b(arg); }
                 );
    return my::clazz::os_str();
}

std::string for_each_objptr(std::initializer_list<my::clazz*> objptr_list,
                            void (my::clazz::*mfn)(int) const,
                            int arg) {
    using std::placeholders::_1;
    std::for_each(objptr_list.begin(), objptr_list.end(),
                  std::bind(mfn, _1, arg)
               // [arg, mfn](const my::clazz* objptr) { (objptr->*mfn)(arg); }
                 );
    return my::clazz::os_str();
}

std::string for_each_mfn(const my::clazz& obj,
                         std::initializer_list<void (my::clazz::*)(int) const> mfnptr_list,
                         int arg) {
    using std::placeholders::_1;
    std::for_each(mfnptr_list.begin(), mfnptr_list.end(),
               // std::bind(_1,  std::cref(obj), arg)
                  [&obj, arg](void (my::clazz::*mfnptr)(int) const) { (obj.*mfnptr)(arg); }
                 );
    return my::clazz::os_str();
}

void bind_vs_lambda() {
    my::clazz x("x");
    my::clazz y("y");
    my::clazz z("z");
    PX(b_for_each_arg(x, {2, 3, 5, 7, 11}));
    PX(b_for_each_obj({x, y, z}, 42));
    PX(for_each_objptr({&x, &y, &z}, &my::clazz::c, 42));
    PX(for_each_mfn(z, {&my::clazz::b, &my::clazz::a, &my::clazz::c}, 42));
}

int main() {
    std::cout.setf(std::ios::boolalpha);
#if 1
    function_basics();
    type_erasure();
    supported_operations();
    function_vs_generic();
    bind_basics();
    ref_and_cref();
    bind_vs_lambda();
    function_callback();
#endif
}
