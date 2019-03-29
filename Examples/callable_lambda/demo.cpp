// http://coliru.stacked-crooked.com/a/64d0447ce5e6dc6e

/*
 * Variations to try:
 * - USE_FUNCTOR_INSTEAD_OF_LAMBDA
 *   in some places where lambdas are used as callables the
 *   alternative functor use is shown as conditionally code
*/

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

bool is_even_fnc(int n) {
    return (n%2 == 0); 
}

struct check_even {
    bool operator()(int n) const {
        return (n%2 == 0); 
    }   
};

struct check_divisible {
    const int by; 
    check_divisible(int by_) : by(by_) {}
    bool operator()(int n) const {
        return (n%by == 0); 
    }   
};

void callable_basics() {
    PX(is_even_fnc(42));        PX(is_even_fnc(3));
    check_even is_even_obj;
    PX(is_even_obj(42));        PX(is_even_obj(3));
    PX(check_even()(42));       PX(check_even()(3));
    PX(check_divisible{2}(42)); PX(check_divisible{2}(3));
//  check_divisible is_even{2};
    auto is_even = check_divisible{2};
    PX(is_even(42));            PX(is_even(3));
    PX(check_divisible{3}(42)); PX(check_divisible{3}(3));
    PX([](int n) { return (n%2 == 0); }(42));
    PX([](int n) { return (n%2 == 0); }(3));
    PX([](int n) { return (n%3 == 0); }(3));
    PX([](int n) { return (n%3 == 0); }(42));
}

#include <sstream>
#include <iterator>
#include <initializer_list>

namespace my {
    template<typename InIt, typename OutIt, typename Pred>
    OutIt copy_if(InIt from, InIt upto, OutIt sink, Pred keep) {
        while (from != upto) {
        //  auto &current = *from;
        //  auto &current{*from};
            const typename std::iterator_traits<InIt>::value_type& current{*from};
            if (keep(current))
                    *sink++ = current;
            ++from;
        }   
        return sink;
    }   
}

std::string capture_nothing(const std::initializer_list<int>& values) {
    std::ostringstream os; 
    my::copy_if(values.begin(), values.end(),
                std::ostream_iterator<int>(os, " "), 
//              is_even_fnc
//              check_even{}
                [](int n) { return (n%2 == 0); }
               );  
    return os.str();
}

#define USE_FUNCTOR_INSTEAD_OF_LAMBDA 1

std::string capture_value(int f, std::initializer_list<int> values) {
    std::ostringstream os; 
    my::copy_if(values.begin(), values.end(),
                std::ostream_iterator<int>(os, " "), 
            #if USE_FUNCTOR_INSTEAD_OF_LAMBDA
                check_divisible{f}
            #else
                [f](int n) { return (n%f == 0); }
            #endif
               );  
    return os.str();
}

#if USE_FUNCTOR_INSTEAD_OF_LAMBDA
struct check_even_count_odd {
    int& odd;
    check_even_count_odd(int& odd_) : odd(odd_) {}
    bool operator()(int n) const {
        if (n%2 == 0)
            return true;
        ++odd;
        return false;
    }   
};
#endif

std::string capture_reference(std::initializer_list<int> values) {
    std::ostringstream os; 
    int dropped{0};
    my::copy_if(values.begin(), values.end(),
                std::ostream_iterator<int>(os, " "), 
            #if USE_FUNCTOR_INSTEAD_OF_LAMBDA
                check_even_count_odd{dropped}
            #else
                [&dropped](int n) {
                    if (n%2 == 0)
                        return true;
                    ++dropped;
                    return false;
                }   
            #endif
               );
    os << "(" << dropped << " dropped)";
    return os.str();
}

#include <set>

bool is_unique(int n) {
    static std::set<int> known;
    return known.insert(n).second;
}

class check_unique {
    std::set<int> known;
public:
    bool operator()(int n) {
        return known.insert(n).second;
    }   
};

std::string capture_init(std::initializer_list<double> values) {
    std::ostringstream os; 
    my::copy_if(values.begin(), values.end(),
                std::ostream_iterator<double>(os, " "), 
        #if 0
                check_unique{}
        #else
                [known = std::set<double>{}](double n) mutable {
                    return known.insert(n).second;
                }   
        #endif
               );  
    return os.str();
}

class check_positive {
public:
    template<typename T>
    bool operator()(T n) {
        return (n > 0); 
    }   
};

template<typename T>
std::string generic_lambda(std::initializer_list<T> values) {
    std::ostringstream os; 
    my::copy_if(values.begin(), values.end(),
                std::ostream_iterator<T>(os, " "), 
        #if 1
                check_positive{}
        #else
             // [](int n) { return n > 0; }
                [](T n) { return n > 0; }
             // [](auto n) { return n > 0; }
        #endif
               );  
    return os.str();
}

void copy_if_demo() {
	    PX(capture_nothing({3, 6, 7, 2, 5, 4, 8, 1, 3, 2}));
    PX(capture_value(2, {3, 6, 7, 2, 5, 4, 8, 1, 3, 2}));
    PX(capture_value(3, {3, 6, 7, 2, 5, 4, 8, 1, 3, 2}));
    PX(capture_reference({3, 6, 7, 2, 5, 4, 8, 1, 3, 2}));
    PX(capture_init({3, 6, 7, 0.2, 5, 4, 8, 1, 3, 2}));
    PX(generic_lambda({8, -9, 10, -11, 12}));
    PX(generic_lambda({.8, -.9, 1.0, -1.1, 2.2}));
}

void capture_basics() {
    static int x = 42;  PX(x); PX(&x);
    //      +------------------------- capture list
    //      |  +---------------------- argument list
    //      |  |        +------------- return type
    //      |  |        |           +- implementation
    //      v  v    vvvvv  vvvvvvvvvv     CALL -vv
    PX(     []() -> void* {return &x;}          ());
    PX(     []() {return "whatever";}           ());
    PX(     []() {return x;}                    ());
    PX(     []() {return &x;}                   ());
    
    auto f1 = []() {return &x;}         (); PT(decltype(f1));
                                         // PX(f1());
    auto f2 = []() {return &x;}           ; PT(decltype(f2));
                                            PX(f2());
    auto f3 = []() -> void* {return &x;}(); PT(decltype(f3));
                                         // PX(f3());
    auto f4 = []() {return (x) ? &x : nullptr;}; PT(decltype(f4));
                                            PX(f4());
//  auto f5 = []() {if (x) return &x; else return nullptr; };
    auto f6 = []() -> const int* {if (x) return &x; else return nullptr;};
    
    int y = 1;  PX(y); PX(&y);
    PX(     [y]() {return y;}                   ());
    PX(     [y]   {return y;}                   ());
    PX(     [y]() mutable {return ++y;}         ());
//  PX(     [y]   mutable {return ++y;}         ());
    PX(     [y]   {return y;}                   ());
    PX(     [&y]() {return ++y;}                ());
    PX(     [y]() {return &y;}                  ());
    PX(     [&y]() {return y;}                  ());
    
    PX(     [=]() {return &y;}                  ());
    PX(     [&]() {return &y;}                  ());
    
    double z = 0;   PX(z); PX(&z);
    PX(     [z = x]() {return z;}               ());
    PX(     [z = &x]() {return &z;}             ());
    PX(     [z = &y]() {return &z;}             ());
    PX(     [z = &z]() {return &z;}             ());
}

#include <array>
void capture_init_array() {
//  std::array<int, 10000> data;
    int data[10000];
    [data](){ PT(decltype(data)); }();
    [&data](){ PT(decltype(data)); }();
//  [d = data]() { PT(decltype(d)); }();
    [d = data]() mutable { PT(decltype(d)); }();
    [d = &data]() { PT(decltype(d)); }();
    [d = &data]() mutable { PT(decltype(d)); }();
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    callable_basics();
//  copy_if_demo();
//  capture_basics();
//  capture_init_array();
}
