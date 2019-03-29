// http://coliru.stacked-crooked.com/a/74ddb26044bb44e9

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "<< (__VA_ARGS__)\
                      << std::endl))
                      
#if 1
#include <chrono>
#include <ratio>
#include <string>
#include <typeinfo>
template<typename T> struct typeprinter
{static std::string str() {return typeid(T).name();}};
template<> struct typeprinter<int>
{static std::string str() {return "int";}};
template<> struct typeprinter<long>
{static std::string str() {return "long";}};
template<> struct typeprinter<long long>
{static std::string str() {return "long long";}};
template<> struct typeprinter<float>
{static std::string str() {return "float";}};
template<> struct typeprinter<double>
{static std::string str() {return "double";}};
template<> struct typeprinter<long double>
{static std::string str() {return "long double";}};
#if 0
template<> struct typeprinter<std::chrono::system_clock>
{static std::string str() {return "sc::system_clock";}};
template<> struct typeprinter<std::chrono::steady_clock>
{static std::string str() {return "sc::steady_clock";}};
template<> struct typeprinter<std::chrono::high_resolution_clock>
{static std::string str() {return "sc::high_resolution_clock";}};
#endif
template<long D, long N> struct typeprinter<std::ratio<D, N>>
{static std::string str() {return "std::ratio<" + std::to_string(D)
                                         + ", " + std::to_string(N) + ">";}};
template<typename T, typename R> struct typeprinter<std::chrono::duration<T, R>>
{static std::string str() {return "sc::duration<" + typeprinter<T>::str()
                                           + ", " + typeprinter<R>::str() + ">";}};
template<typename C, typename D> struct typeprinter<std::chrono::time_point<C, D>>
{static std::string str() {return "sc::time_point<" + typeprinter<C>::str()
                                             + ", " + typeprinter<D>::str() + ">";}};
template<typename T> struct typeprinter<const T>
{static std::string str() {return "const " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<volatile T>
{static std::string str() {return "volatile " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<const volatile T>
{static std::string str() {return "const volatile " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<T*>
{static std::string str() {return typeprinter<T>::str() + "*";}};
template<typename T> struct typeprinter<T&>
{static std::string str() {return typeprinter<T>::str() + "&";}};
template<typename T> struct typeprinter<T&&>
{static std::string str() {return typeprinter<T>::str() + "&&";}};

#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str()\
                      << std::endl))
#else
#include <boost/type_index.hpp>
#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << boost::typeindex::type_id_with_cvr<__VA_ARGS__>()\
                      << std::endl))
#endif                      

// to abbreviate the long namespace prefix without completely
// dropping it the following namespace alias is used in all
// examples:

#include <chrono>
namespace sc = std::chrono;

// also make sure you have a good type printer ready for use,
// otherwise experimenting with chrono types to get a good
// understanding of type conversions results in poor output.

#include <functional>

namespace my {
    template<typename C>
    void show_clock() {
        PT(C);
        PT(typename C::rep);
        PT(typename C::duration);
        PT(typename C::period);
        PX(C::period::num);
        PX(C::period::den);
    }

    int ackermann(int m, int n) {
        return (m == 0) ? n + 1
                        : (n == 0) ? ackermann(m-1, 1)
                                   : ackermann(m-1, ackermann(m, n-1));
    }

    template<typename R = sc::microseconds,
             typename C = sc::high_resolution_clock
    > typename R::rep get_timing_for(std::function<void()> code, int runs = 1) {
        const auto t_start = C::now();
        while (runs-- != 0) code();
        const auto t_end = C::now();
        return sc::duration_cast<R>(t_end - t_start).count();
    }

    template<typename T>
    float days_since_epoch(T t) {
#if 0
        using seconds_per_day = std::ratio<24*60*60>;
        using float_days = sc::duration<float, seconds_per_day>;
        const auto tse = t.time_since_epoch();
        const auto dse = sc::duration_cast<float_days>(tse);
        return dse.count();
#else
        return sc::duration_cast<sc::duration<float, std::ratio<86400>>>(t.time_since_epoch()).count();
    //  return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<86400, 1>>>(t.time_since_epoch()).count();
#endif
    }
}

void cookbook_style() {
    // the current time point
    auto t1 = sc::system_clock::now();
    const auto t1_tomorrow = t1 + sc::hours(24);
    PT(decltype(t1));

//  PX(t1);
    PX(my::days_since_epoch(t1));
    PX(my::days_since_epoch(t1_tomorrow));
    PX(my::days_since_epoch(t1 + sc::hours(24)));
//  const auto t1_tomorrow = t1 + 24h;
//  const auto t1_tomorrow = t1 + 1d;

    // ... let some time pass
    const auto t2 = sc::system_clock::now();
    const auto t_delta = t2-t1;

    PT(decltype(t2));
    PX(my::days_since_epoch(t2));
    PT(decltype(t_delta));
    PX(sc::duration_cast<sc::nanoseconds>(t_delta).count());
}

void system_clock() {
#if 1
    PT(sc::system_clock);
    PT(sc::system_clock::rep);
    PT(sc::system_clock::duration);
    PT(sc::system_clock::period);
    PX(sc::system_clock::period::num);
    PX(sc::system_clock::period::den);
#else
    my::show_clock<sc::system_clock>();
#endif
    const auto t = sc::system_clock::now();
    PT(decltype(t));
//  PX(t);
//  PX(t.count());
    const auto d = sc::system_clock::time_point{}.time_since_epoch();
    PT(decltype(d));
//  PX(d);
    PX(d.count());
}

void steady_clock() {
    my::show_clock<sc::steady_clock>();
    auto t1 = sc::steady_clock::now();
    PX(my::ackermann(3, 4));
    auto t2 = sc::steady_clock::now();
    PX((t2 - t1).count());
}

void high_resolution_clock() {
    my::show_clock<sc::high_resolution_clock>();
    PX(my::get_timing_for(std::bind(my::ackermann, 3, 4)));
//  PX(my::get_timing_for([]{my::ackermann(3, 4);}));
    PX(my::get_timing_for([]{my::ackermann(3, 4);}, 10));
    PX(my::get_timing_for<sc::milliseconds>([]{my::ackermann(3, 4);}, 10));
    using millisec_as_float = sc::duration<float, std::ratio<1, 1000>>;
//  using millisec_as_float = sc::duration<float, std::milli>;
    PT(millisec_as_float);
    auto run_ackermann = []{my::ackermann(3, 4);};
    PX(my::get_timing_for<millisec_as_float>(run_ackermann, 10));
    PX(my::get_timing_for<sc::duration<float, std::ratio<1, 1000>>>([]{my::ackermann(3, 4);}, 10));
    PX(my::get_timing_for<sc::nanoseconds, sc::steady_clock>([]{my::ackermann(3, 4);}));
}

int main() {
    cookbook_style();
//  system_clock();
//  steady_clock();
//  high_resolution_clock();
}
