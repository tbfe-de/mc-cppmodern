// http://coliru.stacked-crooked.com/a/00d2528de2ac01ec

include <iostream>
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
#include <typeinfo>
#define PT(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      <<  "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str()\
                      << std::endl))

template<typename T> struct typeprinter {static const char* str() {return typeid(T).name();}};

template<> struct typeprinter<char> {static const char *str() {return "char";}};
template<> struct typeprinter<short> {static const char *str() {return "short";}};
template<> struct typeprinter<int> {static const char *str() {return "int";}};
template<> struct typeprinter<long> {static const char *str() {return "long";}};
template<> struct typeprinter<long long> {static const char *str() {return "long long";}};

#endif

namespace my {
#if 0
    template<typename... Ts>
    struct types {
        static constexpr auto elem_count() { return sizeof...(Ts); }
    };
#else
    struct notused {};
    template<typename T1 = notused, typename T2 = notused, typename T3 = notused>
    struct types {
        static constexpr std::size_t elem_count() { return 3; }
    };
    template<typename T1, typename T2>
    struct types<T1, T2, notused> {
        static constexpr std::size_t elem_count() { return 2; }
    };
    template<typename T1>
    struct types<T1, notused, notused> {
        static constexpr std::size_t elem_count() { return 1; }
    };
    template<>
    struct types<notused, notused, notused> {
        static constexpr std::size_t elem_count() { return 0; }
    };
#endif
    
    template<int... Vs>
    struct ints {
        static constexpr std::size_t elem_count() {return sizeof...(Vs);}
    };
    
    template<typename T, T... Vs>
    struct values {
        using elem_type = T;
        static constexpr std::size_t elem_count() {return sizeof...(Vs);}
    };
    
//  template<typename... Ts, bool... Bs> struct mixed;
    
}

void the_basics() {
    PX(my::types<int, double, void>::elem_count());
//  PX(my::types<unsigned long, void *>::elem_count());
//  PX(my::types<wchar_t>::elem_count());
//  PX(my::types<>::elem_count());
    PX(my::ints<2, 3, 5, 7, 11, 13>::elem_count());
    
    PT(my::values<char, 'a', 'b', 'c'>::elem_type);
    PX(my::values<char, 'a', 'b', 'c'>::elem_count());
    using bools = my::values<bool, true, false, true||false>;
    PT(bools::elem_type);
    PX(bools::elem_count());
}

namespace my {
    template<typename T, T V>
    struct literal {
        static constexpr T val() { return V; }
    };
    
    template<typename T, T... Vs>
    struct literal_list;
    
    template<typename T, T V1, T... Vs>
    struct literal_list<T, V1, Vs...> {
        static constexpr T head_val() { return V1; }
        using tail_list = literal_list<T, Vs...>;
    };
    
    template<typename T>
    struct literal_list<T> {
    //   using tail_list = literal_list<T>;
    };

}

void literal_lists() {
    PX(my::literal<unsigned int, 42>::val());
    
    using two_by_40 = my::literal<long long, (1LL << 40)>;
    PX(two_by_40::val());
    
    using lili = my::literal_list<int, 3, 7, 11>;
    PX(lili::head_val());
    PX(lili::tail_list::head_val());
    PX(lili::tail_list::tail_list::head_val());
}

#include <limits>
#include <type_traits>

namespace my {
    template<typename... Ts>
    struct type_list {};
    
    template<typename T, typename... Ts>
    struct type_list<T, Ts...> {
        using head_type = T;
        using tail = type_list<Ts...>;
    };

    template<long long M, typename T> struct find_by_max;
    template<long long M, typename T, bool> struct find_helper;
    
    template<long long M, typename T>
    struct find_helper<M, T, true> {
        using result = typename T::head_type;
    };
    template<long long M, typename T>
    struct find_helper<M, T, false> {
        using result = typename find_by_max<M, typename T::tail>::result;
    };
    
    template<long long M, typename T>
    struct find_by_max {
        using head_type = typename T::head_type;
        static const long long Tmax = std::numeric_limits<head_type>::max();
        using result = typename find_helper<M, T, (Tmax >= M)>::result;
    };

}

void type_lists() {
    PT(my::type_list<int, double, void*>);
    
    using tyli = my::type_list<char, short, int, long, long long>;
    PT(tyli::head_type);
    PT(tyli::tail::head_type);
    PT(tyli::tail::tail::head_type);
    PT(tyli::tail::tail::tail::head_type);
    PT(tyli::tail::tail::tail::tail::head_type);
    
    PT(my::find_by_max<127, tyli>::result);
    PT(my::find_by_max<128, tyli>::result);
    PT(my::find_by_max<32767, tyli>::result);
    PT(my::find_by_max<32768, tyli>::result);
}

#include <boost/mpl/vector.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/find.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/bind.hpp>

namespace my {
    template<long long M>
    struct by_max {
        template<typename T>
        struct apply : boost::mpl::bool_<(std::numeric_limits<T>::max() >= M)> {};
    };

    template<long long M>
    struct by_min {
        template<typename T>
        struct apply : boost::mpl::bool_<(std::numeric_limits<T>::min() <= M)> {};
    };
}

void boost_mpl() {
    namespace bm = boost::mpl;
    PT(bm::vector<int, double, void*>);
    
    using tyli = bm::vector<char, short, int, long, long long>;
    PT(tyli);
    PT(bm::front<tyli>::type);
    PT(bm::back<tyli>::type);
    
    PT(bm::deref<bm::find_if<tyli, my::by_max<127>>::type>::type);
    PT(bm::deref<bm::find_if<tyli, my::by_max<128>>::type>::type);
    PT(bm::deref<bm::find_if<tyli, my::by_min<-128>>::type>::type);
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    the_basics();
//  literal_lists();
//  type_lists();
//  boost_mpl();
}
