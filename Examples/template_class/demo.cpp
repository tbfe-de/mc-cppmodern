// http://coliru.stacked-crooked.com/a/f9b919a12f39f613

#include <iostream>

#define PX(expr)\
    ((void)(std::cout << __FILE__  << ':' << __LINE__\
                      << "\t" #expr " --> "\
                      << (expr) << std::endl))

#if 0
namespace my {
    class point {
        double xc, yc;
    public:
        point(double xc_, double yc_) : xc(xc_), yc(yc_) {}
        double x() const {return xc;}
        double y() const {return yc;}
        double x(double x_) {return xc = x_;}
        double y(double y_) {return yc = y_;}
        point shifted(double xdelta, double ydelta) const;
    };
    point point::shifted(double xd, double yd) const  {
        return {xc + xd, yc + yd};
    }
}

int main() {
    std::cout.setf(std::ios::boolalpha);

    my::point a{3.5, 7.0};
    PX(a.x());      PX(a.y());
    PX(a.x(6.0));   PX(a.y(0.0));

    const my::point b{a.shifted(-3.0, 2.5)};
    PX(b.x());      PX(b.y());
//  PX(b.x(6.0));   PX(b.y(0.0));
}

#else

namespace my {
    template<typename T>
    class point {
        T xc, yc;
    public:
        point(T xc_, T yc_) : xc(xc_), yc(yc_) {}
        T x() const {return xc;}
        T y() const {return yc;}
        T x(T x_) {return xc = x_;}
        T y(T y_) {return yc = y_;}
        point shifted(T xd, T yd) const;
    };
#if 1
    template<typename T>
    point<T> point<T>::shifted(T xd, T yd) const {
        return {xc + xd, yc + yd};
    }
#else
    template<typename T>
    auto point<T>::shifted(T xd, T yd) -> point const {
        return {xc + xd, yc + yd};
    }
#endif
}

void point_template() {
    my::point<double> a{3.5, 7.0};
    PX(a.x());      PX(a.y());
    PX(a.x(6.0));   PX(a.y(0.0));

    const my::point<double> b{a.shifted(-3.0, 2.5)};
    PX(b.x());      PX(b.y());
//  PX(b.x(6.0));   PX(b.y(0.0));

    my::point<int> c{10, 20};
    PX(c.x());      PX(c.y());
    
    my::point<bool> d{true, false};
    PX(d.x());      PX(d.y());
}

#include <cctype>
#include <cstring>

namespace my {
    template<std::size_t N>
    class fstring {
        char fs[N+1];
        void copy(const char* cp) {std::strncpy(fs, cp, N)[N] = '\0';}
    public:
        fstring(const char *init) {copy(init);}
        fstring& operator=(const fstring& rhs) {copy(rhs.fs); return *this;}
        operator const char*() const {return fs;};
        char &operator[](std::size_t x) {return fs[x];}
        const char &operator[](std::size_t x) const {return fs[x];}
        std::size_t size() const {return std::strlen(fs);}
        static std::size_t max_size() {return N;}
    };
#if 0
    template<std::size_t N1, std::size_t N2>
    bool operator<(const fstring<N1>& lhs, const fstring<N2> &rhs) {return (std::strcmp(lhs, rhs)) < 0);}
    template<std::size_t N1, std::size_t N2>
    bool operator<=(const fstring<N1>& lhs, const fstring<N2> &rhs) {return (std::strcmp(lhs, rhs)) <= 0);}
    template<std::size_t N1, std::size_t N2>
    bool operator==(const fstring<N1>& lhs, const fstring<N2> &rhs) {return (std::strcmp(lhs, rhs)) == 0);}
    template<std::size_t N1, std::size_t N2>
    bool operator!=(const fstring<N1>& lhs, const fstring<N2> &rhs) {return (std::strcmp(lhs, rhs)) != 0);}
    template<std::size_t N1, std::size_t N2>
    bool operator>=(const fstring<N1>& lhs, const fstring<N2> &rhs) {return (std::strcmp(lhs, rhs)) >= 0);}
    template<std::size_t N1, std::size_t N2>
    bool operator>(const fstring<N1>& lhs, const fstring<N2> &rhs) {return (std::strcmp(lhs, rhs)) > 0);}
#endif
}

void fstring_template() {
    my::fstring<30> a{"hello, world"};	PX(a);
    PX(a.max_size()); PX(a.size());

    PX(a[0]); PX(a[1]); PX(a[2]);
    PX(a[0] = 'H');
    PX(a[7] = std::toupper(a[7]));
    PX(a);

    const my::fstring<3> b{"hi!"};      PX(b);
    PX(b.max_size()); PX(b.size());

    PX(b[0]); PX(b[1]); PX(b[2]);
//  PX(b[0] = a[0]);
    PX(a[0] = b[0]);

    auto c = a;				PX(c);
    PX(a == b); PX(b == c); PX(c == a);
}

#include <string>
namespace my {
    template<typename T> struct typeprinter;
    template<> struct typeprinter<int> {static std::string str() {return "int";}};
    template<> struct typeprinter<double> {static std::string str() {return "double";}};
    template<typename T> struct typeprinter<T*> {static std::string str() {return typeprinter<T>::str() + "*";}};
    template<typename T> struct typeprinter<T&> {static std::string str() {return typeprinter<T>::str() + "&";}};
    template<typename T> struct typeprinter<const T> {static std::string str() {return "const " + typeprinter<T>::str();}};
}

void type_printing() {
    PX(my::typeprinter<int>::str());
    PX(my::typeprinter<double>::str());
    PX(my::typeprinter<const int>::str());
    PX(my::typeprinter<const int *>::str());
    int i = 42;
    PX(i);
    PX(my::typeprinter<decltype(42)>::str());
    PX(my::typeprinter<decltype(i)>::str());
    PX(my::typeprinter<decltype(i+1)>::str());
    PX(my::typeprinter<decltype(&i)>::str());
    const auto &ci = i;
    PX(my::typeprinter<decltype(ci)>::str());
    PX(my::typeprinter<decltype(&ci)>::str());
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    point_template();
    fstring_template();
    type_printing();
}

#endif
