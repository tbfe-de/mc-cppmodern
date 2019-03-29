// http://coliru.stacked-crooked.com/a/15a295275c123079

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))

#if 1
#include <typeinfo>
#include <string>
template<typename T>
struct typeprinter {static std::string str() {return typeid(T).name();}};

#define PT(...)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str() << std::endl))

template<> struct typeprinter<bool> {static std::string str() {return "bool";}};
template<> struct typeprinter<char> {static std::string str() {return "char";}};
template<> struct typeprinter<int> {static std::string str() {return "int";}};
template<> struct typeprinter<float> {static std::string str() {return "float";}};
template<> struct typeprinter<double> {static std::string str() {return "double";}};
template<> struct typeprinter<std::string> {static std::string str() {return "std::string";}};
template<typename T> struct typeprinter<const T> {static std::string str() {return "const " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<T*> {static std::string str() {return typeprinter<T>::str() + "*";}};
template<typename T> struct typeprinter<T&> {static std::string str() {return typeprinter<T>::str() + "&";}};
template<typename T> struct typeprinter<T&&> {static std::string str() {return typeprinter<T>::str() + "&&";}};
#else
#include <boost/type_index.hpp>
#define PT(...)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << boost::typeindex::type_id_with_cvr<__VA_ARGS__>()\
                      << std::endl))
#endif

#include <string>
#include <cmath>
#include <cctype>
#include <tuple>
#include <utility>

void some_basics() {
    std::tuple<char, float, std::string> a{'z', 1/.9f, "hi!"};
    PX(std::get<0>(a));  PT(decltype(std::get<0>(a))); 
    PX(std::get<1>(a));  PT(decltype(std::get<1>(a)));
    PX(std::get<2>(a));  PT(decltype(std::get<2>(a)));

    auto b = std::make_tuple('?', 0.0, "hello");
    PX(std::get<0>(b));  PT(decltype(std::get<0>(b))); 
    PX(std::get<1>(b));  PT(decltype(std::get<1>(b))); 
    PX(std::get<2>(b));  PT(decltype(std::get<2>(b)));

    a = b;    PX(std::get<0>(a)); PX(--std::get<0>(a));
              PX(std::get<1>(a)); PX(std::get<1>(a) = 0.0);
              PX(std::get<2>(a)); PX(&(std::get<2>(a)[0] = 'H'));

    char c; double d; float f; std::string e;
//  std::tie(c, d, e) = a;              PX(c); PX(d), PX(e);
    std::tie(c, f, e) = a;              PX(c); PX(f), PX(e);
    std::tie(std::ignore, d, e) = b;    PX(c); PX(d); PX(e);
}

void create_and_access() {
    std::tuple<char, float, std::string> a{'z', 1/.9f, "hi!"};
//  std::tuple<char, float, std::string> a{'z'};
    PX(std::get<0>(a));  PT(decltype(std::get<0>(a))); 
    PX(std::get<1>(a));  PT(decltype(std::get<1>(a)));
    PX(std::get<2>(a));  PT(decltype(std::get<2>(a)));
//  for (i = 0; i < 3; ++i) PX(std::get<i>(a));

    auto b = std::make_tuple('?', 0.0, "hello");
    PX(std::get<0>(b));  PT(decltype(std::get<0>(b))); 
    PX(std::get<1>(b));  PT(decltype(std::get<1>(b))); 
    PX(std::get<2>(b));  PT(decltype(std::get<2>(b)));

    PX(++std::get<0>(b));
    PX(std::get<1>(b) = std::sqrt(2));
//  PX(std::get<2>(b)[0] = std::toupper(std::get<2>(b)[0]));
    PX(&(std::get<2>(a)[0] = std::toupper(std::get<2>(a)[0])));
}

void assign_and_tie() {
    std::tuple<char, float, std::string> a('z', 1/.9f, "hi!");
//  std::tuple<char, float, std::string> a;
    auto b = std::make_tuple('?', 0.0, "hello");

    PX(++std::get<0>(b));
    PX(std::get<1>(b) = std::sqrt(2));
//  PX(std::get<2>(b)[0] = std::toupper(std::get<2>(b)[0]));
    PX(&(std::get<2>(a)[0] = std::toupper(std::get<2>(a)[0])));

    a = b;
    PX(std::get<0>(a));
    PX(std::get<1>(a));
    PX(std::get<2>(a));

//  b = a;

    char c; double d; float f; std::string e;
//  std::tie(c, d, e) = a;              PX(c); PX(d), PX(e);
    std::tie(c, f, e) = a;              PX(c); PX(f), PX(e);
    std::tie(std::ignore, d, e) = b;    PX(c); PX(d); PX(e);

    PX(f = 0.0); PX(std::get<1>(a));
    PX(std::get<1>(b) = 1e-1); PX(d);
}

void compile_time_ops() {
    using tt = std::tuple<char, float, std::string>;
    PX(std::tuple_size<tt>::value);
    PT(std::tuple_element<0, tt>::type);
    PT(std::tuple_element<1, tt>::type);
    PT(std::tuple_element<2, tt>::type);
#if __plusplus >= 201402L
    PX(std::tuple_size<tt>{});
    PX(std::tuple_element_t<0, tt>);
    PX(std::tuple_element_t<1, tt>);
    PX(std::tuple_element_t<2, tt>);
#endif

    auto b = std::make_tuple('?', 0.0, "hello");
    PX(std::tuple_size<decltype(b)>::value);
    PT(std::tuple_element<0, decltype(b)>::type);
    PT(std::tuple_element<1, decltype(b)>::type);
    PT(std::tuple_element<2, decltype(b)>::type);
#if __cplusplus >= 201402L
    PX(std::tuple_size<decltype(b)>{});
    PT(std::tuple_element_t<0, decltype(b)>);
    PT(std::tuple_element_t<1, decltype(b)>);
    PT(std::tuple_element_t<2, decltype(b)>);
#endif
}

void misc_features() {
    std::tuple<bool> just_one;
    PX(std::tuple_size<decltype(just_one)>::value);
    PT(typename std::tuple_element<0, decltype(just_one)>::type);

//  std::tuple<void> none;

    std::tuple<> empty;
    PX(std::tuple_size<decltype(empty)>::value);

#if __cplusplus >= 201402L
    PX(std::tuple_size<decltype(just_one)>{});
    PT(std::tuple_element_t<0, decltype(just_one)>);
    PX(std::tuple_size<decltype(empty)>{});

    auto b = std::make_tuple('?', 0.0, "hello");
    PT(typename std::tuple_element<0, decltype(b)>::type);
    PX(std::get<char>(b));
    PX(std::get<0>(b));
    PT(typename std::tuple_element<1, decltype(b)>::type);
    PX(std::get<double>(b));
    PX(std::get<1>(b));
    PT(typename std::tuple_element<2, decltype(b)>::type);
    PX(std::get<const char *>(b));
    PX(std::get<2>(b));
#endif
}

#include <set>

void tuple_vs_pair() {
    auto x = std::make_tuple(42, true);
    PX(std::get<0>(x)); PX(std::get<1>(x));
//  PX(x.first); PX(x.second);

    auto y = std::pair<int, bool>{};
    PX(std::get<0>(y)); PX(std::get<1>(y));
    PX(y.first); PX(y.second);

    x = y;
    PX(std::get<0>(x)); PX(std::get<1>(x));

//  y = std::make_tuple(-1, true));
    std::tie(y.first, y.second) = std::make_tuple(-1, 1);
    PX(std::get<0>(y)); PX(std::get<1>(y));
    PX(y.first); PX(y.second);

    std::set<std::string> words{"two"};
#if 0
    PX(*words.insert("three").first);
    PX(words.insert("two").second);
    PX(words.insert("one").second);
#else
    PX(*std::get<0>(words.insert("three")));
    PX(std::get<1>(words.insert("two")));
    PX(std::get<1>(words.insert("one")));
#endif

#if __cplusplus >= 201402L
    PX(std::get<bool>(words.insert("four")));
    PX(std::get<bool>(words.insert("five")));
    PX(std::get<bool>(words.insert("five")));
    PX(*std::get<std::set<std::string>::iterator>(words.insert("five")));
#endif
}

#if __cplusplus > 201402L
#if __has_include(<experimental/tuple>)
#include <experimental/tuple>
void experimental_tuple() {
    using tt = std::tuple<char, float, std::string>;
    PX(std::experimental::tuple_size_v<tt>);
    auto b = std::make_tuple('?', 0.0, "hello");
    PX(std::experimental::tuple_size_v<decltype(b)>);
}
#endif
#else
void experimental_tuple() {}
#endif

int main() {
    std::cout.setf(std::ios::boolalpha);
    some_basics();
//  create_and_access();
//  assign_and_tie();
//  compile_time_ops();
//  misc_features();
//  tuple_vs_pair();
#if __cplusplus > 201402L
//  experimental_tuple();
#endif
}

