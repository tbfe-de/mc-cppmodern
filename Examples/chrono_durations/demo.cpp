// http://coliru.stacked-crooked.com/a/5c6bc3bb243c5545

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
template<> struct typeprinter<std::chrono::system_clock>
{static std::string str() {return "sc::system_clock";}};
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

void predefined_durations() {
    PT(sc::minutes);                PT(sc::minutes::period);
    PX(sc::minutes::period::num);   PX(sc::minutes::period::den);
    PT(sc::minutes::rep);
    PX(sc::minutes{}.count());      PX(sc::minutes::zero().count());
    PX(sc::minutes::min().count()); PX(sc::minutes::max().count());
    {
    //  using dur = sc::nanoseconds;
        using dur = sc::microseconds;
    //  using dur = sc::milliseconds;
    //  using dur = sc::seconds;
    //  using dur = sc::minutes;
    //  using dur = sc::hours;
    //  using dur = sc::days;
        PT(dur);                PT(dur::period);
        PX(dur::period::num);   PX(dur::period::den);
        PT(dur::rep);
        PX(dur{}.count());      PX(dur::zero().count());
        PX(dur::min().count()); PX(dur::max().count());
    }
//  PX(sc::minutes{1});
    PX(sc::minutes{1}.count());
    PX(sc::hours{123}.count());
    PX(sc::microseconds{-4}.count());
//  PX(sc::seconds{0.1}.count());
#if __cplusplus >= 201402L
    using namespace std::chrono_literals;
    PT(decltype(1min)); PX((1min).count());
    PT(decltype(123h)); PX((123h).count());
    PT(decltype(-4us)); PX((-4us).count());
    PT(decltype(0.1s)); PX((0.1s).count());
#endif
}

void nonstandard_durations() {
    using dur1 = sc::duration<short, std::ratio<1, 35>>; PT(dur1);
    using dur2 = sc::duration<short, std::ratio<5, 28>>; PT(dur2);
    using dur3 = sc::duration<float, std::ratio<60>>;    PT(dur3);
    
    dur1 d1{22};    PT(decltype(d1));       PX(d1.count());
    dur2 d2{7};     PT(decltype(d2));       PX(d2.count());
    dur3 d3{1.5};   PT(decltype(d3));       PX(d3.count());
    
    {
        using dur = dur1;
        PT(dur);                PT(dur::period);
        PX(dur::period::num);   PX(dur::period::den);
        PT(dur::rep);
        PX(dur{}.count());      PX(dur::zero().count());
        PX(dur::min().count()); PX(dur::max().count());
    }
}

void combining_durations() {
    sc::hours h{9};             PX(h.count());
    sc::minutes min{h/3};       PX(min.count());    
    sc::seconds sec{min/30};    PX(sec.count());
    sc::milliseconds ms{sec};   PX(ms.count());
    sc::microseconds us{sec};   PX(us.count());
    sc::nanoseconds ns{sec};    PX(ns.count());
    
//  sec = ms;
    sec = sc::duration_cast<sc::seconds>(ms);
    PT(decltype(sec));          PX(sec.count());
    sec = sc::duration_cast<sc::hours>(ms);
    PT(decltype(sec));          PX(sec.count());
    auto z = sc::duration_cast<sc::hours>(min);
    PT(decltype(z));            PX(z.count());
        
    using dur1 = sc::duration<short, std::ratio<1, 35>>; PT(dur1);
    using dur2 = sc::duration<short, std::ratio<5, 28>>; PT(dur2);
    using dur3 = sc::duration<float, std::ratio<60>>;    PT(dur3);
    
    dur1 d1{22};
    dur2 d2{7};     PT(decltype(d2-d1));    PX((d2-d1).count());
    dur3 d3{1.5};   PT(decltype(d3-d1));    PX((d3-d1).count());
    
    d1 = dur1{1};
    d2 = dur2{1};   PT(decltype(d2-d1));    PX((d2-d1).count());
}

void casting_durations() {
    sc::hours h{9};             PX(h.count());
    sc::minutes min{h};         PX(min.count());    
    sc::seconds sec{h};         PX(sec.count());
    sc::milliseconds ms{h};     PX(ms.count());
    
//  sec = ms;
    sec = sc::duration_cast<sc::seconds>(ms);
    PT(decltype(sec));          PX(sec.count());
    sec = sc::duration_cast<sc::hours>(ms);
    PT(decltype(sec));          PX(sec.count());
    auto z = sc::duration_cast<sc::hours>(min);
    PT(decltype(z));            PX(z.count());
    
    using dur1 = sc::duration<short, std::ratio<1, 35>>;
    using dur2 = sc::duration<short, std::ratio<5, 28>>;
    using dur3 = sc::duration<float, std::ratio<60>>;
    
    dur1 d1{100};                           PX(d1.count());
//  dur2 d2{d1};                            PX(d2.count());    
    dur2 d2{sc::duration_cast<dur2>(d1)};   PX(d2.count());
//  auto d2 = sc::duration_cast<dur2>(d1);  PX(d2.count());
    auto d3 = sc::duration_cast<dur3>(d1);  PX(d3.count());
}

namespace my {
    template<typename T>
    std::string dur2str(const T& t) {
        using namespace std;
        using namespace std::chrono;
        const auto d = duration_cast<seconds>(t);
        return to_string(duration_cast<hours>(d).count() / 24) + "d"
             + to_string(duration_cast<hours>(d).count() % 24) + "h"
             + to_string(duration_cast<minutes>(d % hours{1}).count()) + "m"
             + to_string(duration_cast<seconds>(d % minutes{1}).count()) + "s"
             ;
    }
    template<typename R, typename T>
    std::string dur2(const T&t) {
        using namespace std;
        using namespace std::chrono;
        using dur = duration<long double, typename R::period>;
        return to_string(duration_cast<dur>(t).count());
    }
}

void printing_durations() {
    const auto z = sc::hours(1)
                 + sc::minutes(14)
              // + sc::minutes(100)
                 + sc::seconds(59)
                 + sc::milliseconds(500)
	         + sc::nanoseconds(12)
                 ;
    PT(decltype(z));
    PX(sc::duration_cast<sc::hours>(z).count());
    PX(sc::duration_cast<sc::minutes>(z % sc::hours{1}).count());
    PX(sc::duration_cast<sc::seconds>(z % sc::minutes{1}).count());
    PX(sc::duration_cast<sc::milliseconds>(z % sc::seconds{1}).count());
    PX(sc::duration_cast<sc::nanoseconds>(z % sc::seconds{1}).count());

    using fp_dur = sc::duration<float, std::ratio<3600,1>>;
    PX(sc::duration_cast<fp_dur>(z).count());
    PX(fp_dur{1}.count());
    PX((fp_dur{1}/4).count());
    PX(z.count());
    PX((z + fp_dur{0}).count());

    PX(my::dur2str(z));
    PX(my::dur2<sc::hours>(z));
    PX(my::dur2<sc::seconds>(z));
    PX(my::dur2<sc::milliseconds>(z));
}

int main() {
    predefined_durations();
    nonstandard_durations();
    combining_durations();
    casting_durations();
    printing_durations();
}
