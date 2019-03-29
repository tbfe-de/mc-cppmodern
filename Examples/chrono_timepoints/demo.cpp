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

namespace my {
    using lhs_duration = sc::seconds;
    using rhs_duration = sc::seconds;
//  using rhs_duration = sc::minutes;
    using lhs_time_point = sc::time_point<int, sc::seconds>;
    using rhs_time_point = sc::time_point<int, sc::seconds>;
//  using rhs_time_point = sc::time_point<int, sc::milliseconds>;
}


void additive_demo() {
    PT(decltype(  my::lhs_duration{}   + my::rhs_duration{}     ));
    PT(decltype(  my::lhs_duration{}   + my::rhs_time_point{}   ));
    PT(decltype(  my::lhs_time_point{} + my::rhs_duration{}     ));
//  PT(decltype(  my::lhs_time_point{} + my::rhs_time_point{}   ));
    PT(decltype(  my::lhs_duration{}   - my::rhs_duration{}     ));
//  PT(decltype(  my::lhs_duration{}   - my::rhs_time_point{}   ));
    PT(decltype(  my::lhs_time_point{} - my::rhs_duration{}     ));
    PT(decltype(  my::lhs_time_point{} - my::rhs_time_point{}   ));
}

void multiplicative_demo() {
    PT(decltype(  my::lhs_duration{}   * int{}                  ));
    PT(decltype(  my::lhs_duration{}   * float{}                ));
    PT(decltype(  int{}                * my::rhs_duration{}     ));
    PT(decltype(  float{}              * my::rhs_duration{}     ));
    PT(decltype(  my::lhs_duration{}   / my::rhs_duration{}     ));
    PT(decltype(  my::lhs_duration{}   / int{}                  ));
    PT(decltype(  my::lhs_duration{}   / float{}                ));
    PT(decltype(  my::lhs_duration{}   % int{}                  ));
    PT(decltype(  my::lhs_duration{}   % my::rhs_duration{}     ));
}

void mixed_demo() {
    PT(decltype(  int{} * sc::hours{} / 3.5                     ));
    PT(decltype(  int{} * sc::seconds{} / 3.5                   ));
    PT(decltype(  sc::seconds{} / sc::seconds{} / 3.5           ));
    PT(decltype(  sc::seconds{} % sc::seconds{} / 3             ));
    PT(decltype(  sc::seconds{} % (sc::seconds{} / 3)           ));
    PT(decltype(  int{} * sc::hours{} % sc::minutes{}
                        + sc::system_clock::time_point{}));
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    additive_demo();
//  multiplicative_demo();
//  mixed_demo();
}
