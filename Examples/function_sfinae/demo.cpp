// http://coliru.stacked-crooked.com/a/39d4970b9e3fa520

#include <iostream>
#include <string>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))

#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str() << std::endl))

template<typename T> struct typeprinter;
#define FOR_EACH_ARITHMETIC_TYPE(GEN_m)\
    GEN_m(         bool       , integral_signed  )\
    GEN_m(         char       , integral_signed  )\
    GEN_m(signed   char       , integral_signed  )\
    GEN_m(unsigned char       , integral_unsigned)\
    GEN_m(signed   short      , integral_signed  )\
    GEN_m(unsigned short      , integral_unsigned)\
    GEN_m(signed   int        , integral_signed  )\
    GEN_m(unsigned int        , integral_unsigned)\
    GEN_m(signed   long       , integral_signed  )\
    GEN_m(unsigned long       , integral_unsigned)\
    GEN_m(signed   long long  , integral_signed  )\
    GEN_m(unsigned long long  , integral_unsigned)\
    GEN_m(         float      , floating_point   )\
    GEN_m(         double     , floating_point   )\
    GEN_m(         long double, floating_point   )

#define GEN_typeprinter(base_type, kind_of)\
    template<> struct typeprinter<base_type>\
    {static std::string str() {return #base_type;}};
 // -----------------------------------------
    FOR_EACH_ARITHMETIC_TYPE(GEN_typeprinter)
 // -----------------------------------------
#undef GEN_typeprinter

#include <type_traits>

namespace my {
#if 1
    template<typename T>
    void foo_integral_unsigned(T arg) {
        PT(decltype(arg)); //PT(T);
    }
    template<typename T>
    void foo_integral_signed(T arg) {
        PT(decltype(arg)); //PT(T);
    }
    template<typename T>
    void foo_floating_point(T arg) {
        PT(decltype(arg)); //PT(T);
    }
    #if 0
    void foo(bool arg)               {PT(decltype(arg)); foo_integral_signed(arg);}
    void foo(char arg)               {PT(decltype(arg)); foo_integral_signed(arg);}
    void foo(signed char arg)        {PT(decltype(arg)); foo_integral_signed(arg);}
    void foo(unsigned char arg)      {PT(decltype(arg)); foo_integral_unsigned(arg);}
    void foo(signed short arg)       {PT(decltype(arg)); foo_integral_signed(arg);}
    void foo(unsigned short arg)     {PT(decltype(arg)); foo_integral_unsigned(arg);}
    void foo(signed int arg)         {PT(decltype(arg)); foo_integral_signed(arg);}
    void foo(unsigned int arg)       {PT(decltype(arg)); foo_integral_unsigned(arg);}
    void foo(signed long arg)        {PT(decltype(arg)); foo_integral_signed(arg);}
    void foo(unsigned long arg)      {PT(decltype(arg)); foo_integral_unsigned(arg);}
    void foo(signed long long arg)   {PT(decltype(arg)); foo_integral_signed(arg);}
    void foo(unsigned long long arg) {PT(decltype(arg)); foo_integral_unsigned(arg);}
    void foo(float arg)              {PT(decltype(arg)); foo_floating_point(arg);}
    void foo(double arg)             {PT(decltype(arg)); foo_floating_point(arg);}
    void foo(long double arg)        {PT(decltype(arg)); foo_floating_point(arg);}
    #else
    #define GEN_delegation(base_type, kind_of)\
        void foo(base_type arg) {PT(decltype(arg)); foo_ ## kind_of(arg);}
     // -----------------------------------------
        FOR_EACH_ARITHMETIC_TYPE(GEN_delegation)
     // -----------------------------------------
    #undef GEN_delegation
    #endif
#else
    template<typename T>
    typename std::enable_if<
       std::is_unsigned<T>::value
    >::type foo(T integral_unsigned_arg) {
        PT(decltype(integral_unsigned_arg)); //PT(T);
    }
    
    template<typename T>
    typename std::enable_if<
       std::is_signed<T>::value && !std::is_floating_point<T>::value
    >::type foo(T integral_signed_arg) {
        PT(decltype(integral_signed_arg)); //PT(T);
    }
    
    template<typename T>
    typename std::enable_if<
       std::is_floating_point<T>::value
    >::type foo(T floating_point_arg) {
        PT(decltype(floating_point_arg)); //PT(T);
    }
#endif
}

void sfinae_return_type() {
#if 1
    {using bt = bool;               my::foo(bt{});};
    {using bt = signed char;        my::foo(bt{});};
    {using bt = unsigned char;      my::foo(bt{});};
    {using bt = signed short;       my::foo(bt{});};
    {using bt = unsigned short;     my::foo(bt{});};
    {using bt = signed int;         my::foo(bt{});};
    {using bt = unsigned int;       my::foo(bt{});};
    {using bt = signed long;        my::foo(bt{});};
    {using bt = unsigned long;      my::foo(bt{});};
    {using bt = signed long long;   my::foo(bt{});};
    {using bt = unsigned long long; my::foo(bt{});};
    {using bt = float;              my::foo(bt{});};
    {using bt = double;             my::foo(bt{});};
    {using bt = long double;        my::foo(bt{});};
#else
#define GEN_test_call(basic_type, kind_of)\
    {using bt = basic_type; my::foo(bt{});}
 // ---------------------------------------
    FOR_EACH_ARITHMETIC_TYPE(GEN_test_call)
 // ---------------------------------------
#undef GEN_test_call
#endif
}

#if __cplusplus > 201402L
#include <experimental/type_traits>
#endif

namespace my {
#if 1
    template<typename T>
    void bar(T signed_arg,
             typename std::enable_if<
                 std::is_signed<T>::value
             >::type* = nullptr
    ) {
        PT(decltype(signed_arg)); //PT(T);
    }
    template<typename T>
    void bar(T unsigned_arg,
             typename std::enable_if<
             //  std::is_unsigned<T>::value
                 !std::is_signed<T>::value
             >::type* = nullptr
   ) {
        PT(decltype(unsigned_arg)); //PT(T);
    }
#elif __cplusplus < 201402L
    template<typename T>
    using enable_for_unsigned = std::enable_if<std::is_unsigned<T>::value>;
    template<typename T>
    void bar(T unsigned_arg, typename enable_for_unsigned<T>::type* = nullptr) {
        PT(decltype(unsigned_arg)); //PT(T);
    }

    template<typename T>
    using enable_for_signed = std::enable_if<!std::is_signed<T>::value>;
    template<typename T>
    void bar(T signed_arg, typename enable_for_signed<T>::type* = nullptr) {
        PT(decltype(signed_arg)); //PT(T);
    }
#elif __cplusplus == 201402L
    template<typename T>
    void bar(T signed_arg,
             typename std::enable_if_t<
                 std::is_signed<T>{}
             >* = nullptr
    ) {
        PT(decltype(signed_arg)); //PT(T);
    }
    template<typename T>
    void bar(T unsigned_arg,
             typename std::enable_if_t<
             //  std::is_unsigned<T>{}
                 !(std::is_signed<T>{})
             >* = nullptr
    ) {
        PT(decltype(unsigned_arg)); //PT(T);
    }
#elif __cplusplus > 201402L
    template<typename T>
    void bar(T signed_arg,
             typename std::enable_if_t<
                 std::experimental::is_signed_v<T>
             >* = nullptr
    ) {
        PT(decltype(signed_arg)); //PT(T);
    }
    template<typename T>
    void bar(T unsigned_arg,
             typename std::enable_if_t<
             //  std::is_unsigned_v<T>
                 !std::experimental::is_signed_v<T>
             >* = nullptr
    ) {
        PT(decltype(unsigned_arg)); //PT(T);
    }
#endif
}

void sfinae_default_arg() {
#define GEN_test_call(basic_type, kind_of)\
    {using bt = basic_type; my::bar(bt{});}
 // ---------------------------------------
    FOR_EACH_ARITHMETIC_TYPE(GEN_test_call)
 // ---------------------------------------
#undef GEN_test_call
}

int main() {
//  sfinae_return_type();
    sfinae_default_arg();
}

