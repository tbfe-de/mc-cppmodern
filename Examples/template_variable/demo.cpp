#include <iostream>                    
#include <string> 
    
template<typename> struct typeprinter; 

using namespace std::literals::string_literals;
template<> struct typeprinter<float>
{static auto str() {return "float"s;}};
template<> struct typeprinter<double>
{static auto str() {return "double"s;}};
template<> struct typeprinter<long double>
{static auto str() {return "long double"s;}};
template<typename T> struct typeprinter<const T>
{static std::string str() {return "const "s + typeprinter<T>::str();}};
template<typename T> struct typeprinter<T &>
{static std::string str() {return typeprinter<T>::str() + "&"s;}};

#define PT(t)\
    std::cout << #t " --> " << typeprinter<t>::str() << std::endl

#define PX(t)\
    std::cout << #t " --> " << (t) << std::endl
    
#include <cmath>
//#include <math.h>

namespace my {
    template<typename T> static const T PI;
    template<> const double PI<float> = 2*std::acos(0.f);
    template<> const double PI<double> = 2*std::acos(0.0);
    template<> const double PI<long double> = 2*std::acos(0.L);
//  template<> const double PI<long double> = 2*acosl(0.L);
}

int main() {
    std::cout.precision(10);
    
    PT(      decltype(2*std::acos(0.f)));
    PT(      decltype(my::PI<float>));
    PX(               my::PI<float> );
    PX(std::to_string(my::PI<float>));

    PT(      decltype(2*std::acos(0.0)));
    PT      (decltype(my::PI<double>));
    PX(               my::PI<double> );
    PX(std::to_string(my::PI<double>));

    PT(      decltype(2*std::acos(0.L)));
    PT(      decltype(my::PI<long double>));
    PX(               my::PI<long double> );
    PX(std::to_string(my::PI<long double>));
    
    PX(my::PI<float> - my::PI<double>);
    PX(my::PI<double> - my::PI<long double>);
}
