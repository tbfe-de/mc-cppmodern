// http://coliru.stacked-crooked.com/a/2e3c17c27b91709b

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

int main() {
    PT(sc::seconds);  PT(sc::minutes);  PT(sc::hours);

    const auto d1 = sc::seconds{12};    PT(decltype(d1));
    const auto& d2 = sc::hours{2};      PT(decltype(d2)); 
                                        PT(decltype(d2 - d1));
    auto t1 = sc::system_clock::now();  PT(decltype(t1));
    auto&& t2 = t1.time_since_epoch();  PT(decltype(t2));

    PT(decltype(t1 + sc::minutes{3}));
    PT(decltype(t2 + sc::minutes{3}));
}
