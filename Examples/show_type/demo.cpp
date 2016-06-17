// http://coliru.stacked-crooked.com/a/4907e3cbb1ed8c81

#include <iostream>

template<typename T> struct showtype;

// In the following the recognition of cv-qualifiers combined
// with pointers and references is too simplisic and does not
// reflect precedence in the generated type name - the example
// is to demonstrate the basic technique only and NOT meant
// to work as reliable type printer!

#if 0
    
template<> struct showtype<void>
{static std::string str() {return "void";}};
template<> struct showtype<bool>
{static std::string str() {return "bool";}};
template<> struct showtype<char>
{static std::string str() {return "char";}};
template<> struct showtype<wchar_t>
{static std::string str() {return "wchar_t";}};
template<> struct showtype<signed char>
{static std::string str() {return "signed char";}};
template<> struct showtype<unsigned char>
{static std::string str() {return "unsigned char";}};
template<> struct showtype<signed short>
{static std::string str() {return "signed short";}};
template<> struct showtype<unsigned short>
{static std::string str() {return "unsigned short";}};
template<> struct showtype<signed int>
{static std::string str() {return "signed int";}};
template<> struct showtype<unsigned int>
{static std::string str() {return "unsigned int";}};
template<> struct showtype<signed long>
{static std::string str() {return "signed long";}};
template<> struct showtype<unsigned long>
{static std::string str() {return "unsigned long";}};
template<> struct showtype<signed long long>
{static std::string str() {return "signed long long";}};
template<> struct showtype<unsigned long long>
{static std::string str() {return "unsigned long long";}};
template<> struct showtype<float>
{static std::string str() {return "float";}};
template<> struct showtype<double>
{static std::string str() {return "double";}};
template<> struct showtype<long double>
{static std::string str() {return "long double";}};
template<> struct showtype<std::nullptr_t>
{static std::string str() {return "std::nullptr_t";}};

template<typename T> struct showtype<const T>
{static std::string str() {return "const " + showtype<T>::str();}};
template<typename T> struct showtype<volatile T>
{static std::string str() {return "volatile " + showtype<T>::str();}};
template<typename T> struct showtype<const volatile T>
{static std::string str() {return "const volatile " + showtype<T>::str();}};

template<typename T> struct showtype<T*>
{static std::string str() {return showtype<T>::str() + "*";}};
template<typename T> struct showtype<T&>
{static std::string str() {return showtype<T>::str() + "&";}};
template<typename T> struct showtype<T&&>
{static std::string str() {return showtype<T>::str() + "&&";}};

#else

#define GEN_plain_basic_type(nm)\
    template<> struct showtype<nm>\
    {static std::string str() {return #nm;}};
#define GEN_signed_unsigned(nm)\
    GEN_plain_basic_type(signed nm)\
    GEN_plain_basic_type(unsigned nm)
#define GEN_const_volatile(cv)\
    template<typename T> struct showtype<cv T>\
    {static std::string str() {return #cv " " + showtype<T>::str();}};
#define GEN_ref_and_ptr(rp)\
    template<typename T> struct showtype<T rp>\
    {static std::string str() {return showtype<T>::str() + #rp;}};

GEN_plain_basic_type(void)
GEN_plain_basic_type(bool)
GEN_plain_basic_type(char)
GEN_plain_basic_type(wchar_t)

GEN_signed_unsigned(char)
GEN_signed_unsigned(short)
GEN_signed_unsigned(int)
GEN_signed_unsigned(long)
GEN_signed_unsigned(long long)

GEN_plain_basic_type(float)
GEN_plain_basic_type(double)
GEN_plain_basic_type(long double)

GEN_plain_basic_type(std::nullptr_t)

GEN_const_volatile(const)
GEN_const_volatile(volatile)
GEN_const_volatile(const volatile)

GEN_ref_and_ptr(&)
GEN_ref_and_ptr(&&)
GEN_ref_and_ptr(*)

#undef GEN_plain_basic_type
#undef GEN_signed_unsigned
#undef GEN_const_volatile
#undef GEN_ref_and_ptr

#endif

template<typename T> struct w;

#define PT(type) std::cout << #type << " --> "\
                           << showtype<type>::str() << std::endl;
                           
int main() {
    int* p = nullptr;
    using mytype = int;
    PT(int);
    PT(decltype(p));
    PT(decltype(&p));
    PT(decltype(*p));
    PT(void*);
    PT(decltype(nullptr));
    PT(const char **);
    PT(volatile long *const *&);
}
