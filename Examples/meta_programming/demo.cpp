#include <iostream>

namespace my {
    template<typename T>
    struct add_ptr { using type = T*; };
    
    template<typename T>
    struct remove_ptr;
    template<typename T>
    struct remove_ptr<T*> {using type = T;};

    template<typename T>
    struct remove_all_ptr {using type = T;};
    template<typename T>
    struct remove_all_ptr<T*> {
    //  using type = remove_all_ptr<T>;
        using type = typename remove_all_ptr<T>::type;
    };

    template<unsigned N>
    struct countbits {
        static const std::size_t value = (N & 0x1) + countbits<(N>>1)>::value;
    };
    template<>
    struct countbits<unsigned{0}> {
        static const std::size_t value = 0;
    };

    template<typename T>
    struct is_ptr {static const bool value{false};};
    template<typename T>
    struct is_ptr<T*> {static const bool value{true};};

}
    
#if 0

template<typename T> struct typeprinter;

int main() {
//  -------------------------------------------- "input" is:

//  typeprinter< my::add_ptr<int>::type           > _;
//                           ^^^---------------------- type

//  typeprinter< my::remove_ptr<int**>::type      > _;
//                              ^^^^^----------------- type

//  typeprinter< my::remove_all_ptr<int***>::type > _;
//                                  ^^^^^^------------ type

    std::cout << my::countbits<42>::value << std::endl;
//                             ^^--------------------- value
}

#else

#include <string>

template<typename> struct typeprinter;

template<> struct typeprinter<int>
{static std::string str() {return "int";}};
// ... and more "mutatis mutandis"

template<typename T> struct typeprinter<T*>
{static std::string str() {return typeprinter<T>::str() + "*";}};
// ... and more "mutatis mutandis"

template<typename T> struct typeprinter<const T>
{static std::string str() {return "const " + typeprinter<T>::str();}};
// ... and more "mutatis mutandis"

void bare_bones_use() {
    std::cout
    << typeprinter<my::add_ptr<int>::type>::str()           << '\n'
    << typeprinter<my::remove_ptr<int**>::type>::str()      << '\n'
    << typeprinter<my::remove_all_ptr<int***>::type>::str() << '\n'
    <<             my::countbits<42>::value                 << '\n'
    ;
}

#define PX(expr)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #expr " --> "\
                      << (expr) << std::endl))
#define PT(type)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #type " --> "\
                      << typeprinter<type>::str() << std::endl))

void macro_based_use() {
    PT(my::add_ptr<int>::type);
    PT(my::remove_ptr<int**>::type);
    PT(my::remove_all_ptr<int***>::type);
    PX(my::countbits<42>::value);
}

void branches_and_loops() {
    PX(my::is_ptr<int>::value);
//  PT(my::remove_ptr<int>::type);
    PT(my::remove_all_ptr<int>::type);
    PX(my::is_ptr<int*>::value);
    PT(my::remove_ptr<int*>::type);
    PT(my::remove_all_ptr<int*>::type);
    PX(my::is_ptr<int*****>::value);
    PT(my::remove_all_ptr<int*****>::type);
    PT(my::remove_ptr<int*****>::type);

    int i{42};
    PX(my::is_ptr<decltype(i)>::value);
    PX(my::is_ptr<decltype(&i)>::value);

    const int *p = &i;
    PX(my::is_ptr<decltype(p)>::value);
    PX(my::is_ptr<decltype(*p)>::value);
    PX(my::is_ptr<decltype(p[0])>::value);
    PX(my::is_ptr<decltype(&p)>::value);
    PT(my::remove_ptr<decltype(&p)>::type);
    PT(my::remove_all_ptr<decltype(&p)>::type);
}

int main() {
    bare_bones_use();
    macro_based_use();
    branches_and_loops();
}

#endif
