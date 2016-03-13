// http://coliru.stacked-crooked.com/a/393450ac53867965

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

#if 1
template<> struct typeprinter<sc::nanoseconds>
{static std::string str() {return "sc::nanoseconds";}};
template<> struct typeprinter<sc::microseconds>
{static std::string str() {return "sc::microseconds";}};
template<> struct typeprinter<sc::milliseconds>
{static std::string str() {return "sc::milliseconds";}};
template<> struct typeprinter<sc::seconds>
{static std::string str() {return "sc::seconds";}};
template<> struct typeprinter<sc::minutes>
{static std::string str() {return "sc::minutes";}};
template<> struct typeprinter<sc::hours>
{static std::string str() {return "sc::hours";}};
#endif

#if 1
template<> struct typeprinter<std::tm>
{static std::string str() {return "struct tm";}};
#endif

namespace my {
    using lhs_duration = sc::seconds;
    using rhs_duration = sc::seconds;
//  using rhs_duration = sc::minutes;
    using lhs_time_point = sc::time_point<int, sc::seconds>;
    using rhs_time_point = sc::time_point<int, sc::seconds>;
//  using rhs_time_point = sc::time_point<int, sc::milliseconds>;
}

namespace my {
    int ackermann(int m, int n) {
        return (m == 0) ? n + 1
                        : (n == 0) ? ackermann(m-1, 1)
                                   : ackermann(m-1, ackermann(m, n-1));
    }
}

#include <ctime>

void chrono_vs_time_t() {
    auto tp_chrono = sc::system_clock::now();
    PT(decltype(tp_chrono));
//  PX(tp_chrono);
    PX(tp_chrono.time_since_epoch().count());
    using long_days = sc::duration<long, std::ratio<24*60*60>>;
    PT(long_days);
    PX(sc::duration_cast<long_days>(tp_chrono.time_since_epoch()).count());

    auto tp_cstyle = sc::system_clock::to_time_t(tp_chrono);
    PT(decltype(tp_cstyle));
    PX(tp_cstyle);

//  auto tp_struct_tm = std::gmtime(&tp_cstyle);
    auto tp_struct_tm = std::localtime(&tp_cstyle);
//  PT(decltype(tp_struct_tm));
    PX(tp_struct_tm->tm_sec);
    PX(tp_struct_tm->tm_min);
    PX(tp_struct_tm->tm_hour);
    PX(tp_struct_tm->tm_mday);
    PX(tp_struct_tm->tm_mon);
    PX(tp_struct_tm->tm_year);
    PX(tp_struct_tm->tm_wday);
    PX(tp_struct_tm->tm_yday);
    PX(tp_struct_tm->tm_isdst);

    char buff[100];
    PX(std::strftime(buff, sizeof buff, "%Y-%m-%d %H:%M:%S (%a)", tp_struct_tm), buff);
    PX(std::strftime(buff, sizeof buff, "%F %T %Z", tp_struct_tm), buff);
    PX(std::strftime(buff, sizeof buff, "%c", tp_struct_tm), buff);

    PX(tp_cstyle += (26*7) * (24*60*60));
//  tp_struct_tm = std::gmtime(&tp_cstyle);
    tp_struct_tm = std::localtime(&tp_cstyle);
    PX(std::strftime(buff, sizeof buff, "%c", tp_struct_tm), buff);
#if 0
    PX(tp_struct_tm->tm_sec);
    PX(tp_struct_tm->tm_min);
    PX(tp_struct_tm->tm_hour);
    PX(tp_struct_tm->tm_mday);
    PX(tp_struct_tm->tm_mon);
    PX(tp_struct_tm->tm_year);
    PX(tp_struct_tm->tm_wday);
    PX(tp_struct_tm->tm_yday);
#endif
    PX(tp_struct_tm->tm_isdst);

    tp_chrono = sc::system_clock::from_time_t(tp_cstyle);
    PX(tp_chrono.time_since_epoch().count());
    PX(sc::duration_cast<long_days>(tp_chrono.time_since_epoch()).count());
}

void chrono_vs_clock_t() {
    PT(std::clock_t);
    std::clock_t t1 = std::clock(); PX(t1);
    std::clock_t t2 = t1;           PX(t2);
    int i = 0;
    for (i = 4; t2 == t1;) {
          my::ackermann(3, i++);
          t1 = std::clock();
    }
    PX(t1 = std::clock());
    PX(i); PX(my::ackermann(3, i));
    PX(t2 = std::clock());
//  PT(decltype(t2-t1));          PX(t2-t1);
    PT(decltype(CLOCKS_PER_SEC)); PX(CLOCKS_PER_SEC);
    PX("in msec =", 1e3*(t2-t1) / CLOCKS_PER_SEC);

#if 0
    using clock_t_ratio = std::ratio<1, CLOCKS_PER_SEC>;
    PT(clock_t_ratio);
    using clock_duration = sc::duration<std::clock_t, clock_t_ratio>;
#else
    using clock_duration = sc::duration<std::clock_t, std::ratio<1, CLOCKS_PER_SEC>>;
#endif
    PT(clock_duration);
    clock_duration delta{t2-t1};
    using float_usec = sc::duration<float>;
    PX(sc::duration_cast<float_usec>(delta).count());
}

int main() {
    chrono_vs_time_t();
    chrono_vs_clock_t();
}
