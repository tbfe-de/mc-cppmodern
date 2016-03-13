// http://coliru.stacked-crooked.com/a/27f12f6e0e51fe47

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))
#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str() << std::endl))

#include <string>
#include <typeinfo>
template<typename T> struct typeprinter {
    static std::string str() {return typeid(T).name();}
};
template<> struct typeprinter<short> {static std::string str() {return "short";}};
template<> struct typeprinter<unsigned short> {static std::string str() {return "unsigned short";}};
template<> struct typeprinter<int> {static std::string str() {return "int";}};
template<> struct typeprinter<unsigned int> {static std::string str() {return "unsigned int";}};
template<> struct typeprinter<long> {static std::string str() {return "long";}};
template<> struct typeprinter<unsigned long> {static std::string str() {return "unsigned long";}};
template<> struct typeprinter<float> {static std::string str() {return "float";}};
template<> struct typeprinter<double> {static std::string str() {return "double";}};
template<typename T> struct typeprinter<T&> { static std::string str() {return typeprinter<T>::str() + "&";}};
template<typename T> struct typeprinter<T&&> { static std::string str() {return typeprinter<T>::str() + "&&";}};
template<typename T> struct typeprinter<T*> { static std::string str() {return typeprinter<T>::str() + "*";}};
template<typename T> struct typeprinter<const T> { static std::string str() {return "const " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<volatile T> { static std::string str() {return "volatile " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<const volatile T> { static std::string str() {return "const volatile " + typeprinter<T>::str();}};

namespace my {
    template<bool B> struct boolean {static constexpr bool value{B};};
}
template<> struct typeprinter<my::boolean<true>>
{static std::string str() {return "my:boolean<true>";}};
template<> struct typeprinter<my::boolean<false>>
{static std::string str() {return "my:boolean<false>";}};

namespace my {
    template<typename T> struct fits_reg       : boolean<false> {};
    template<> struct fits_reg<short>          : boolean<true> {};
    template<> struct fits_reg<unsigned short> : boolean<true> {};
    template<> struct fits_reg<int>            : boolean<true> {};
    template<> struct fits_reg<unsigned int>   : boolean<true> {};

    template<typename T> struct raw_equiv;
    template<> struct raw_equiv<short>  {using type = unsigned int;};
    template<> struct raw_equiv<int>    {using type = unsigned int;};
    template<> struct raw_equiv<long>   {using type = unsigned long;};
    template<> struct raw_equiv<float>  {using type = unsigned int;};
    template<> struct raw_equiv<double> {using type = unsigned long;};
}

void traits_style_1() {
    PX(my::fits_reg<short>::value);
    PX(my::fits_reg<int>::value);
    PX(my::fits_reg<double>::value);

    PT(my::raw_equiv<int>::type);
    PT(my::raw_equiv<float>::type);
    PT(my::raw_equiv<double>::type);
}

namespace my {
    template<typename T> struct bt_info;

    template<> struct bt_info<short> {
        static const bool fits_reg{true}; using raw_equiv = unsigned int;
    };
    template<> struct bt_info<unsigned short> {
        static const bool fits_reg{true}; using raw_equiv = unsigned int;
    };
    template<> struct bt_info<int> {
        static const bool fits_reg{true}; using raw_equiv = unsigned int;
    };
    template<> struct bt_info<unsigned int> {
        static const bool fits_reg{true}; using raw_equiv = unsigned int;
    };
    template<> struct bt_info<long> {
        static const bool fits_reg{false}; using raw_equiv = unsigned long;
    };
    template<> struct bt_info<unsigned long> {
        static const bool fits_reg{false}; using raw_equiv = unsigned long;
    };
    template<> struct bt_info<float> {
        static const bool fits_reg{true}; using raw_equiv = unsigned int;
    };
    template<> struct bt_info<double> {
        static const bool fits_reg{false}; using raw_equiv = unsigned long;
    };
}

void traits_style_2() {
    PX(my::bt_info<short>::fits_reg);
    PX(my::bt_info<int>::fits_reg);
    PX(my::bt_info<double>::fits_reg);

    PT(my::bt_info<int>::raw_equiv);
    PT(my::bt_info<float>::raw_equiv);
    PT(my::bt_info<double>::raw_equiv);
}

#include <type_traits>
#if __cplusplus > 201402L
#include <experimental/type_traits>
#endif

void standard_traits() {
    int i{0};   	  PT(decltype(i));
                          PT(decltype(*&i));

    const auto &cri = i;  PT(decltype(cri));
                          PT(decltype(*&cri));

#if 1
    PX(std::is_const<decltype(cri)>::value);
    PT(std::remove_const<decltype(cri)>::type);
    PX(std::is_reference<decltype(cri)>::value);
    PT(std::remove_reference<decltype(cri)>::type);

    PX(std::is_const<decltype(i)>::value);
    PT(std::remove_const<decltype(i)>::type);
    PX(std::is_reference<decltype(i)>::value);
    PT(std::remove_reference<decltype(i)>::type);

    PX(std::is_const<const volatile int>::value);
    PT(std::remove_const<const volatile int>::type);

    PX(std::is_same<signed int, int>::value);
    PX(std::is_same<signed char, char>::value);
    PX(std::is_same<unsigned char, char>::value);
    
    PT(std::conditional<true, int**, double&>::type);
    PT(std::conditional<false, int**, double&>::type);
#else
    PX(std::is_const<decltype(cri)>{});
    PT(std::remove_const_t<decltype(cri)>);
    PX(std::is_reference<decltype(cri)>{});
    PT(std::remove_reference_t<decltype(cri)>);

    PX(std::is_const<decltype(i)>{});
    PT(std::remove_const_t<decltype(i)>);
    PX(std::is_reference<decltype(i)>{});
    PT(std::remove_reference_t<decltype(i)>);

    PX(std::is_const<const volatile int>{});
    PT(std::remove_const_t<const volatile int>);

    PX(std::is_same<signed int, int>{});
    PX(std::is_same<signed char, char>{});
    PX(std::is_same<unsigned char, char>{});
    
    PT(std::conditional_t<true, int**, double&>);
    PT(std::conditional_t<false, int**, double&>);
    
    #if __cplusplus > 201402L
    PX(std::experimental::is_const_v<decltype(i)>);
    PX(std::experimental::is_const_v<decltype(cri)>);
    PX(std::experimental::is_reference_v<decltype(i)>);
    PX(std::experimental::is_reference_v<decltype(cri)>);
    PX(std::experimental::is_same_v<unsigned char, char>);
    #endif
#endif
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    traits_style_1();
    traits_style_2();
    standard_traits();
}

