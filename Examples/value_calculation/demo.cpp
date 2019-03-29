// http://coliru.stacked-crooked.com/a/b8fb5d149600d9bb

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
template<> struct typeprinter<bool> {static std::string str() {return "bool";}};
template<> struct typeprinter<char> {static std::string str() {return "char";}};
template<> struct typeprinter<signed char> {static std::string str() {return "signed char";}};
template<> struct typeprinter<unsigned char> {static std::string str() {return "unsigned char";}};
template<> struct typeprinter<int> {static std::string str() {return "int";}};
template<> struct typeprinter<double> {static std::string str() {return "double";}};

namespace my {
    constexpr unsigned gcd_function(unsigned m, unsigned n) {
        return n ? gcd_function(n, m % n) : m;
    }

    template<unsigned M, unsigned N>
    struct gcd_template {
        static constexpr unsigned value = gcd_template<N, M % N>::value;
    };
    template<unsigned M>
    struct gcd_template<M, 0> {
        static constexpr unsigned value = M;
    };
}

void function_test() {
    PX(my::gcd_function(42, 8));
    PX(my::gcd_function(42, 7));
    PX(my::gcd_function(42, 6));
    PX(my::gcd_function(42, 5));
    PX(my::gcd_function(42, 4));
    PX(my::gcd_function(42, 2));
    PX(my::gcd_function(6, 7));
    PX(my::gcd_function(7, 6));
    PX(my::gcd_function(126, 7));
    PX(my::gcd_function(126, 6));
    PX(my::gcd_function(126, 1));
    PX(my::gcd_function(126, 0));
    PX(my::gcd_function(1, 0));
    PX(my::gcd_function(0, 1));
    PX(my::gcd_function(0, 0));
}

void template_test() {
//  PX(my::gcd_template<42, 8>);
    PT(my::gcd_template<42, 8>);
    PX(my::gcd_template<42, 8>::value);
    PX(my::gcd_template<42, 7>::value);
    PX(my::gcd_template<42, 6>::value);
    PX(my::gcd_template<42, 5>::value);
    PX(my::gcd_template<42, 4>::value);
    PX(my::gcd_template<42, 3>::value);
    PX(my::gcd_template<42, 2>::value);
    PX(my::gcd_template<6, 7>::value);
    PX(my::gcd_template<6, 7>::value);
    PX(my::gcd_template<126, 7>::value);
    PX(my::gcd_template<126, 6>::value);
    PX(my::gcd_template<126, 1>::value);
    PX(my::gcd_template<126, 0>::value);
    PX(my::gcd_template<1, 0>::value);
    PX(my::gcd_template<0, 1>::value);
    PX(my::gcd_template<0, 0>::value);
}

namespace my {
    template<typename T, T V = T{}>
    struct constant {
    //  using type = constant<T, V>;
    //  using value_type = T;
        static constexpr T value = V;
    };
#if 0
    template<bool B>
    struct boolean {
    //  using type = bool<B>;
    //  using value_type = bool;
        static constexpr bool value = B;
    };
    template<int I>
    struct integer {
    //  using type = integer<I>;
    //  using value_type = int;
        static constexpr int value = I;
     };
#else
    #if 1 
    template<bool B> struct boolean : constant<bool, B> {};
    template<int I> struct integer : constant<int, I> {};
    #else
    template<bool B> using boolean<B> = constant<bool, B>;
    template<int I> using integer<I> = constant<int, I>;
    #endif
#endif
}

#if 1 // enable boolean conversion to std::string
namespace std {
    std::string to_string(bool b) {return b ? "true" : "false";}
}
#endif

#if 1 // enable more detailed specialisations
#if 1    // enable my::constant<bool, ...>
template<>
//struct typeprinter<my::constant<bool, true>>
  struct typeprinter<my::boolean<true>>
  {static std::string str() {return "my::boolean<true>";}};
template<>
//struct typeprinter<my::constant<bool, false>>
  struct typeprinter<my::boolean<false>>
  {static std::string str() {return "my::boolean<false>";}};
#endif  // ------- my::constant<bool, ...>

template<int I>
struct typeprinter<my::integer<I>> {
    static std::string str() {
        return "my::integer<" + std::to_string(I) + ">";
    }
};
#endif // ----- more detailed specialisations

template<typename T, T I>
struct typeprinter<my::constant<T, I>> {
    static std::string str() {
        return "my::constant<" + typeprinter<T>::str()
                        + ", " + std::to_string(I) + ">";
    }
};

void value_as_type() {
    PT(my::constant<bool, true>);
    PX(my::constant<bool, true>::value);
    PT(my::constant<bool, false>);
    PX(my::constant<bool, false>::value);

    PT(my::constant<int, 42>);
    PX(my::constant<int, 42>::value);

    PT(my::boolean<true>);
    PX(my::boolean<true>::value);
    PT(my::boolean<false>);
    PX(my::boolean<false>::value);

    PT(my::integer<101>);
    PX(my::integer<101>::value);

#if 0
    PT(my::boolean<true>::type);
    PT(my::boolean<false>::type);
    PT(my::integer<0>::type);
    PT(my::constant<int, -1>::type);

    PT(my::boolean<true>::value_type);
    PT(my::boolean<false>::value_type);
    PT(my::integer<0>::value_type);
    PT(my::constant<int, -1>::value_type);
#endif
}

namespace my {
#if 1
    template<typename T1, typename T2>
    struct is_same {
         using type = boolean<false>;
         static constexpr bool value = type::value;
    };
    template<typename T1>
    struct is_same<T1, T1> {
         using type = boolean<true>;
         static constexpr bool value = type::value;
    };
#else
    template<typename T1, typename T2>
    struct is_same : boolean<false> {};
    template<typename T1>
    struct is_same<T1, T1> : boolean<true> {};
#endif
}

template<typename T1, typename T2>
struct typeprinter<my::is_same<T1, T2>> {
    static std::string str() {
        return "my::is_same<" + typeprinter<T1>::str()
                       + ", " + typeprinter<T2>::str() + ">";
    }
};

void compare_types() {
    PX(my::is_same<double, char>::value);
    PX(my::is_same<char, char>::value);
    PX(my::is_same<char, signed char>::value);
    PX(my::is_same<char, unsigned char>::value);

    PT(my::is_same<double, char>::type);
    PT(my::is_same<char, char>::type);
    PT(my::is_same<char, signed char>::type);
    PT(my::is_same<char, unsigned char>::type);
}

int main() {
    std::cout.setf(std::ios::boolalpha);
//  function_test();
//  template_test();
//  value_as_type();
//  compare_types();
}
