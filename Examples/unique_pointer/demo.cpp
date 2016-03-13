// http://coliru.stacked-crooked.com/a/fbef0dab3cdd9059

#include <iostream>
#include <string>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ":" << __LINE__\
                      << "\t"  #__VA_ARGS__ " --> " << (__VA_ARGS__)\
                      << std::endl))
                      
template<typename> struct typeprinter;
template<> struct typeprinter<int>
{static std::string str() {return "int";}};
template<> struct typeprinter<double>
{static std::string str() {return "double";}};
template<typename T> struct typeprinter<T[]>
{static std::string str() {return typeprinter<T>::str() + "[]";}};
template<typename T> struct typeprinter<T *>
{static std::string str() {return typeprinter<T>::str() + "*";}};
template<typename T> struct typeprinter<T &>
{static std::string str() {return typeprinter<T>::str() + "&";}};
#if 0
template<typename T> struct typeprinter<const T>
{static std::string str() {return "const " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<T &&>
{static std::string str() {return typeprinter<T>::str() + "&&";}};
#endif

#define PT(...)\
    ((void)(std::cout << __FILE__ ":" << __LINE__\
                      << "\t"  #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str()\
                      << std::endl))
#include <memory>

namespace my {
    struct clazz {
        int x;
        int get_x() const {return x;}
        int set_x(int x_) {return (x = x_);}
    #define P(t)\
        std::cout << "=> my::clazz " t " @" << this << std::endl;
        clazz()                 {P("default c'tor")}
        clazz(int x_) : x(x_)   {P("(int) c'tor")}
        clazz(const clazz&)     {P("copy c'tor")}
        clazz(clazz&&)          {P("move c'tor")}
        ~clazz()                {P("d'tor")}
        clazz& operator=(const clazz&) =delete;
        clazz& operator=(clazz&&) =delete;
        clazz(bool b)           {P("(bool) c'tor");
                                 if (b) throw "c'tor failed";}
    #undef P
        char sizer[20];
    };

    std::unique_ptr<clazz> factory(int a, int b) {
       return std::unique_ptr<clazz>{new clazz(a*b)};
    }
}

template<> struct typeprinter<my::clazz>
{static std::string str() {return "my::clazz";}};

void unique_ptr_basics() {
    PX(sizeof(my::clazz));
    PX(sizeof(my::clazz*));

    std::unique_ptr<my::clazz> ptr{new my::clazz{42}};
    PX(sizeof(ptr));
    PX(sizeof(*ptr));   PT(decltype(*ptr));
//  PX(sizeof(ptr[0])); PT(decltype(ptr[0]));


    std::unique_ptr<my::clazz[]> arr{new my::clazz[3]{}};
    PX(sizeof(arr));
//  PX(sizeof(*arr));   PT(decltype(*arr));
    PX(sizeof(arr[0])); PT(decltype(arr[0]));

//  std::unique_ptr<my::clazz> pt2{ptr};
    std::unique_ptr<my::clazz> pt2{std::move(ptr)};

    std::unique_ptr<my::clazz> pt3{my::factory(6, 7)};
}

#if (__cplusplus < 201402L)
namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
#endif

void make_unique_ptr() {
    {
        auto ptr = std::make_unique<my::clazz>();
        PX(ptr.get());
        auto pt2 = std::make_unique<my::clazz>(6*7);
        PX(pt2.get());
    }
    std::unique_ptr<my::clazz[]> arr;
    PX(arr.get());
    
#if (__cplusplus >= 201402L)
    arr = std::make_unique<my::clazz[]>(std::size_t{3});
    PX(arr.get());  PX(&arr[0]);  PX(&arr[1]); PX(&arr[2]);
    
    arr = std::make_unique<my::clazz[]>(std::size_t{2});
    PX(arr.get());  PX(&arr[0]);  PX(&arr[1]); 
#endif
}

void access_unique_ptr() {
    {
        auto ptr = std::make_unique<my::clazz>(5);
        PX(ptr->get_x());
        PX(ptr->set_x(2));
        PX(ptr->x);
    }
#if (__cplusplus >= 201402L)
    {
        auto arr = std::make_unique<my::clazz[]>(2);
        PX(arr.get());
        PX(arr[0].x++);
        PX(arr[1].set_x(arr[0].get_x()));
    }
#endif
    {
        auto pt2 = std::make_unique<double>(1.0/9.0);
        PX(*pt2);
    }
}

void test_unique_ptr() {
    {
        std::unique_ptr<my::clazz> ptr;         PX((bool)ptr); PX(ptr.get());
        ptr = std::make_unique<my::clazz>();    PX((bool)ptr); PX(ptr.get());
        auto pt2 = std::move(ptr);              PX((bool)pt2); PX(pt2.get());
                                                PX((bool)ptr); PX(ptr.get());
        pt2 = nullptr;                          PX((bool)pt2); PX(pt2.get());
    }
#if (__cplusplus >= 201402L)
    auto arr = std::make_unique<my::clazz>();   PX((bool)arr); PX(arr.get());
    arr = nullptr;                              PX((bool)arr); PX(arr.get());
#endif
}

void legacy_unique_ptr() {
    auto ptr = std::make_unique<my::clazz>(5);      PX(ptr.get());
    auto pt2 = my::factory(10, 10);                 PX(pt2.get());
    
    auto p = ptr.get();     PT(decltype(p)); PX(p); PX(ptr.get());
    auto q = pt2.release(); PT(decltype(q)); PX(q); PX(pt2.get());
                            PT(decltype(*p)); PX(p->x);
                            PT(decltype(*q)); PX(q->x);
//  delete p;
    delete q;
}

void unique_ptr_pitfalls() {
    int x = 0xDEADBEEF;                     PX(&x);     PX(x);
    auto ptr = std::unique_ptr<int>{&x};    PX(&*ptr);  PX(*ptr);
    auto p = new int{6*7};                  PX(p);      PX(*p);
    auto pt1 = std::unique_ptr<int>{p};     PX(&*pt1);  PX(*pt1);
//  auto pt2 = std::unique_ptr<int>{p};
//  auto pt3 = std::unique_ptr<int>{pt1.get()};
//  delete p;
//  std::unique_ptr<my::clazz> ptr{new my::clazz[5]};
//  std::unique_ptr<my::clazz[]> arr{new my::clazz(5)};
}

#include <cstring>

namespace my {
    class point {
        std::unique_ptr<char[]> nm;
        double xc, yc;
    public:
        point(const char *name, double xc_ = 0.0, double yc_ = 0.0)
            : nm(std::strcpy(new char[std::strlen(name)+1], name))
            , xc(xc_), yc(yc_)
        {}
        friend std::ostream& operator<<(std::ostream &lhs, const point &rhs) {
	    lhs << "point{nm=" << (rhs.nm ? &rhs.nm[0] : "??")
                << ", xc=" << rhs.xc << ", yc=" << rhs.yc << "}";
	}
    };
}

void unique_ptr_as_member() {
    my::point x{"first", 3.0, 4.5};	PX(x);
    my::point y{"other"};		PX(y);
//  my::point z{x};
    my::point z{std::move(x)};		PX(z); PX(x);
//  x = y;
    x = std::move(y);			PX(x); PX(y);
}

int main() {
    std::cout.setf(std::ios::boolalpha);
#if 0
    unique_ptr_basics();
    make_unique_ptr();
    test_unique_ptr();
    access_unique_ptr();
    legacy_unique_ptr();
    unique_ptr_pitfalls();
#endif
    unique_ptr_as_member();
}
