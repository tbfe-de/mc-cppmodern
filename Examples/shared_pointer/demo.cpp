// http://coliru.stacked-crooked.com/a/0fc58836466e4dd6

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
    ((void)(std::cout << __FUNCTION__ << ":" << __LINE__\
                      << "\t"  #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str()\
                      << std::endl))
#include <memory>

namespace my {
    std::ostream &operator<<(std::ostream &lhs, const struct clazz*);
    
    struct clazz {
        int x;
        int get_x() const {return x;}
        int set_x(int x_) {return (x = x_);}
        std::shared_ptr<clazz> get_self() {
                return std::shared_ptr<clazz>(this);
        }
    #define P(t)\
        std::cout << "\t=> my::clazz " t << this << std::endl;
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

    std::shared_ptr<clazz> factory(int a, int b) {
       return std::shared_ptr<clazz>{new clazz(a*b)};
    }
    
    std::ostream &operator<<(std::ostream &lhs, const clazz *rhs) {
    //  return lhs << '@' << (rhs ? (void*)rhs : "@nullptr");
        if (rhs)
            return lhs << '@' << (void*)rhs;
        else
            return lhs << "@nullptr";
    }
}

template<> struct typeprinter<my::clazz>
{static std::string str() {return "my::clazz";}};

void shared_ptr_basics() {
    PX(sizeof(my::clazz));
    PX(sizeof(my::clazz*));

    std::shared_ptr<my::clazz> ptr{new my::clazz{42}}; 
    PX(sizeof(ptr)); PX(sizeof(*ptr)); PT(decltype(*ptr)); 
    PX(ptr.get());  PX(ptr.use_count());  PX(ptr.unique());
    
    std::shared_ptr<my::clazz> pt2{ptr};
    PX(pt2.get());  PX(pt2.use_count());  PX(pt2.unique());
    PX(ptr.get());  PX(ptr.use_count());  PX(ptr.unique());

    std::shared_ptr<my::clazz> pt3;
    PX(pt3.get());  PX(pt3.use_count());  PX(pt3.unique());
    
    pt3 = my::factory(6, 7);
    PX(pt3.get());  PX(pt3.use_count());  PX(pt3.unique());
    
    pt3 = nullptr;
//  pt3.reset();
    PX(pt3.get());  PX(pt3.use_count());  PX(pt3.unique());
    
    pt3 = pt2;
    PX(pt3.get());  PX(pt3.use_count());  PX(pt3.unique());
    PX(pt2.get());  PX(pt2.use_count());  PX(pt2.unique());
    PX(ptr.get());  PX(ptr.use_count());  PX(ptr.unique());
}

void make_shared_ptr() {
    auto ptr = std::make_shared<my::clazz>();
    PX(ptr.get()); PX(ptr.use_count()); PX(ptr.unique());
    
    auto pt2 = std::make_shared<my::clazz>(6*7);
    PX(pt2.get());
    std::shared_ptr<my::clazz> data[5];
    for (auto &e : data) {
        PX("before", e.use_count());
        PX((e = ptr).get());
        PX("after", e.use_count());
    }
    PX(ptr.use_count()); PX(ptr.unique()); PX(ptr.get());
    ptr.reset();
    PX(ptr.use_count()); PX(ptr.unique()); PX(ptr.get());
    for (auto &e : data) {
        PX("before", e.use_count());
        PX((e = nullptr).get());
        PX("after", e.use_count());
    }
}

#include <vector>
void test_shared_ptr() {
    std::vector<std::shared_ptr<my::clazz>> data(3);
    {
        std::shared_ptr<my::clazz> ptr;         PX((bool)ptr);  PX(ptr.get());
        ptr = std::make_shared<my::clazz>();    PX((bool)ptr);  PX(ptr.get());
        auto pt2 = std::move(ptr);              PX((bool)pt2);  PX(pt2.get());
                                                PX((bool)ptr);  PX(ptr.get());
        PX(pt2.use_count()); PX(pt2.unique());
        for (auto &e : data) {
            PX((e = pt2).use_count());          PX((bool)pt2);  PX(pt2.get());
        }
        pt2 = nullptr;                          PX((bool)pt2);  PX(pt2.get());
        while (!data.empty()) {
        //  if (data.front().unique()) break;
            const auto &z = data.back();        PX((bool)z);    PX(z.get());
            PX(data.pop_back(), z.use_count());
        }
        PX(data.size());
    }
}

void access_shared_ptr() {
    std::vector<std::shared_ptr<my::clazz>> data;
    {
        auto ptr = std::make_shared<my::clazz>(5);
        PX(ptr->get_x());
        PX(ptr->set_x(2));
        PX(ptr->x);
        for (auto i = 1; i <= 3; ++i)  {
        //  data.push_back(ptr);
            data.push_back(std::make_shared<my::clazz>(*ptr));
            PX(data.back()->set_x(2*i+1));
            PX((*ptr).get_x());
        //  PX(ptr->get_x());
        }
    }
    auto pt2 = std::make_shared<double>(1.0/9.0);
    PX(*pt2);
}

#include <cstdio>

void custom_deleter() {
    std::shared_ptr<std::FILE> out_fp{
        std::fopen("testfile.txt", "w"),
        [](FILE *fp) {
            std::cout << "\t==> closing output file @"
                      << (void*)fp << std::endl;;
            if (fp) std::fclose(fp);
        }
    };
    PX(out_fp.get());
    if (out_fp.get()) {
        std::fputs("hello, world", out_fp.get());
//      std::fflush(out_fp.get());
    }
//  out_fp.reset();
    std::shared_ptr<std::FILE> in_fp{
        std::fopen("testfile.txt", "r"),
        [](FILE *fp) {
            std::cout << "\t==> closing input file @"
                      << (void*)fp << std::endl;
            if (fp) std::fclose(fp);
        }
    };
    PX(in_fp.get());
    if (in_fp.get()) {
        char line[80] = {'\0'};
        PX(std::fgets(line, sizeof line, in_fp.get()) ? line : "<eof>");
    }
}

void shared_ptr_pitfalls() {
    int x = 0xDEADBEEF;                     PX(&x);     PX(x);
//  auto ptr = std::shared_ptr<int>{&x};    PX(&*ptr);  PX(*ptr);
    auto p = new int{6*7};                  PX(p);      PX(*p);
    auto pt1 = std::shared_ptr<int>{p};     PX(&*pt1);  PX(*pt1);
//  auto pt2 = std::shared_ptr<int>{p};
//  auto pt3 = std::shared_ptr<int>{pt1.get()};
//  delete p;
    auto ptz = std::make_shared<my::clazz>();
//  auto pts = ptz->get_self();
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    shared_ptr_basics();
//  make_shared_ptr();
//  test_shared_ptr();
//  access_shared_ptr();
//  custom_deleter();
//  shared_ptr_pitfalls();
}
