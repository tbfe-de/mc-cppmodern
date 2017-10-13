// http://coliru.stacked-crooked.com/a/45634dc767e71332

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t"  #__VA_ARGS__ " --> " << (__VA_ARGS__)\
                      << std::endl))

#if __cplusplus > 201402L
#if __has_include(<boost/type_index.hpp>)
#include <boost/type_index.hpp>
#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      <<  "\t" #__VA_ARGS__ " --> "\
                      << boost::typeindex::type_id_with_cvr<__VA_ARGS__>()\
                      << std::endl));
#endif
#endif

#ifndef PT
#include <string>
#include <typeinfo>
#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      <<  "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str()\
                      << std::endl))

template<typename T> struct typeprinter {static const char* str() {return typeid(T).name();}};

template<> struct typeprinter<void> {static std::string str() {return "void";}};
template<> struct typeprinter<bool> {static std::string str() {return "bool";}};
template<> struct typeprinter<char> {static std::string str() {return "char";}};
template<> struct typeprinter<short> {static std::string str() {return "short";}};
template<> struct typeprinter<int> {static std::string str() {return "int";}};
template<> struct typeprinter<long> {static std::string str() {return "long";}};
template<> struct typeprinter<long long> {static std::string str() {return "long long";}};

template<typename T> struct typeprinter<T*>
{static std::string str() {return typeprinter<T>::str() + "*";}};


#endif

namespace my {
    template<typename... Ts>
    struct tuple;
    
    template<>
    struct tuple<> {};
#if 0
    #define ITH_TYPE_ONLY
    template<typename T1>
    struct tuple<T1> {
        T1 m1;
        tuple(const T1& m1_) : m1(m1_) {}
    };
    template<typename T1, typename T2>
    struct tuple<T1, T2> {
        T1 m1;
        T2 m2;
        tuple(const T1& m1_, const T2& m2_) : m1(m1_), m2(m2_) {}
    };
    template<typename T1, typename T2, typename T3>
    struct tuple<T1, T2, T3> {
        T1 m1;
        T2 m2;
        T3 m3;
        tuple(const T1& m1_, const T2& m2_, const T3& m3_) : m1(m1_), m2(m2_), m3(m3_) {}
    };
    template<std::size_t I, typename T> struct ith_type;
    #if 0
    template<typename T1>
    struct ith_type<0, tuple<T1>> {
        using type = T1;
    };
    template<typename T1, typename T2>
    struct ith_type<0, tuple<T1, T2>> {
        using type = T1;
    };
    template<typename T1, typename T2, typename T3>
    struct ith_type<0, tuple<T1, T2, T3>> {
        using type = T1;
    };
    // ------------------------------------
    template<typename T1, typename T2>
    struct ith_type<1, tuple<T1, T2>> {
        using type = T2;
    };
    template<typename T1, typename T2, typename T3>
    struct ith_type<1, tuple<T1, T2, T3>> {
        using type = T2;
    };
    //  ---------------------------------------
    template<typename T1, typename T2, typename T3>
    struct ith_type<2, tuple<T1, T2, T3>> {
        using type = T3;
    };
    #else
    template<typename T1, typename... Ts>
    struct ith_type<0, tuple<T1, Ts...>> {
        using type = T1;
    };
//  ---------------------------------------
    template<typename T1, typename T2, typename... Ts>
    struct ith_type<1, tuple<T1, T2, Ts...>> {
        using type = T2;
    };
//  ---------------------------------------
    template<typename T1, typename T2, typename T3, typename... Ts>
    struct ith_type<2, tuple<T1, T2, T3, Ts...>> {
        using type = T3;
    };
    #endif
//  ------------------------------------------
#elif 0 // tuple with data recursion via member
    template<typename T1, typename... Ts>
    struct tuple<T1, Ts...> {
        T1 m1;
        tuple<Ts...> ms;
        tuple(const T1& m1_, const Ts&... ms_) : m1(m1_), ms(ms_...) {}
    };
    template<std::size_t I, typename T>
    struct ith_type;
    template<std::size_t I, typename T1, typename... Ts>
    struct ith_type<I, tuple<T1, Ts...>> {
        using type = typename ith_type<I-1, tuple<Ts...>>::type; 
    };
    template<typename T1, typename... Ts>
    struct ith_type<0, tuple<T1, Ts...>> {
        using type = T1;
    };
    template<std::size_t I, typename T>
    auto get(const T& arg) -> typename std::enable_if<I != 0, typename ith_type<I, T>::type>::type {
        return get<I-1>(arg.ms);
    }
    template<std::size_t I, typename T>
    auto get(const T& arg) -> typename std::enable_if<I == 0, typename ith_type<I, T>::type>::type {
        return arg.m1;
    }
#else // tuple with data recursion via base class
    template<std::size_t I, typename T>
    struct ith_type;
    template<typename T1, typename... Ts>
    struct tuple<T1, Ts...> : private tuple<Ts...> {
        T1 m1;
        tuple(const T1& m1_, const Ts&... ms_) : m1(m1_), tuple<Ts...>(ms_...) {}
        template<std::size_t I_, typename T_, typename... Ts_>
        friend
        auto get(const tuple<T_, Ts_...>&) -> typename ith_type<I_, tuple<T_, Ts_...>>::type;
    };
    template<std::size_t I, typename T1, typename... Ts>
    struct ith_type<I, tuple<T1, Ts...>> {
        using type = typename ith_type<I-1, tuple<Ts...>>::type; 
    };
    template<typename T1, typename... Ts>
    struct ith_type<0, tuple<T1, Ts...>> {
        using type = T1;
    };
    template<std::size_t I, typename T, typename... Ts>
    auto get(const tuple<T, Ts...>& arg) -> typename std::enable_if<I != 0, typename ith_type<I, tuple<T, Ts...>>::type>::type {
        return get<I-1>(static_cast<const tuple<Ts...> &>(arg));
    }
    template<std::size_t I, typename T, typename... Ts>
    auto get(const tuple<T, Ts...>& arg) -> typename std::enable_if<I == 0, typename ith_type<I, tuple<T, Ts...>>::type>::type {
        return arg.m1;
    }

#endif
    template<typename... Ts>
    auto make_tuple(Ts&&... args) -> tuple<Ts...> {
    //  return {std::forward<Ts&&...>(args...)};
        return tuple<Ts...>{std::forward<Ts&&...>(args...)};
    }
   
}

#ifndef ITH_TYPE_ONLY
#include <string>
#include <sstream>
#include <utility>

namespace my {
#if __cplusplus < 201402L
    std::string to_string(const tuple<> &) {
        return std::string{};
    }
    template<typename... Ts>
    std::string to_string(const tuple<Ts...> &t) {
        std::ostringstream s;
        s << t.m1;
        if (sizeof...(Ts) > 1)
            s << ", " << to_string(t.ms);
        return s.str();
    }
#else
    template<typename... Ts, std::size_t... Is>
    auto to_string_helper(const tuple<Ts...> &t, std::index_sequence<Is...> = std::index_sequence_for<Ts...>{}) {
        std::ostringstream s;
    #if __cplusplus < 201703L
        using left_to_right = int[];
        static_cast<void>(
            // only the side effects of the following are of interest
            left_to_right{0, // <-- required in case of empty expansion
                (void(s << (Is == 0? "" : ", ") << get<Is>(t)), 0)...
            }                      // here "runs" the loop(!) ----^^^
        );
    #else
        ((s << (Is == 0? "" : ", ") << get<Is>(t)), ...);
    #endif
        return s.str();
    }
    template<typename... Ts>
    inline auto to_string(const tuple<Ts...>& t) {
        return to_string_helper(t, std::index_sequence_for<Ts...>{});
    }
#endif
}
#endif

void tuple_creation() {
    my::tuple<> t0;
    PT(decltype(t0));

    my::tuple<char> t1{'z'};
    PT(decltype(t1));

    my::tuple<bool, int> t2{false, 42};
    PT(decltype(t2));

    my::tuple<bool, int, void*> t3{true, 12, nullptr};
    PT(decltype(t3));
#if 0
    PX(t1.m1);

    PX(t2.m1);
    PX(t2.m2);

    PX(t3.m1);
    PX(t3.m2);
    PX(t3.m3);
#endif
}

void ith_type_test() {
    my::tuple<> t0;
    PT(decltype(t0));
    
    my::tuple<char> t1{'z'};
    PT(decltype(t1));
    PT(my::ith_type<0, decltype(t1)>::type);

    my::tuple<bool, int> t2{false, 42};
    PT(decltype(t2));
    PT(my::ith_type<0, decltype(t2)>::type);
    PT(my::ith_type<1, decltype(t2)>::type);

    my::tuple<bool, int, void*> t3{true, 12, nullptr};
    PT(decltype(t3));
    PT(my::ith_type<0, decltype(t3)>::type);
    PT(my::ith_type<1, decltype(t3)>::type);
    PT(my::ith_type<2, decltype(t3)>::type);
}

void element_access() {
#ifndef ITH_TYPE_ONLY
    my::tuple<> t0;
    PT(decltype(t0));
    
    my::tuple<char> t1{'z'};
    PT(decltype(t1));

    my::tuple<bool, int> t2{false, 42};
    PT(decltype(t2));

    my::tuple<bool, int, void*> t3{true, 12, nullptr};
    PT(decltype(t3));

    #if 1
    PX(my::get<0>(t1));
    PX(my::to_string(t1));
    
    PX(my::get<0>(t2));
    PX(my::get<1>(t2));
    PX(my::to_string(t2));
    
    PX(my::get<0>(t3));
    PX(my::get<1>(t3));
    PX(my::get<2>(t3));
    PX(my::to_string(t3));
    #else
    PX(t1.m1);
    PX(t2.m1); PX(t2.m2);
    PX(t3.m1); PX(t3.m2); PX(t3.m3);
    #endif
#endif
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    tuple_creation();
    ith_type_test();
    element_access();
}

