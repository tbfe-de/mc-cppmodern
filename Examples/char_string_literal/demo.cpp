// http://coliru.stacked-crooked.com/a/c03827eb62cf51cb

#include <iostream>
#include <string>

template<typename T> struct typeprinter;

#define FOR_EACH_CHAR_TYPE(x)\
        x(char)              \
        x(signed char)       \
        x(unsigned char)     \
        x(wchar_t)           \
        x(char16_t)          \
        x(char32_t)
        
#define DEF_typeprinter(t)\
    template<> struct typeprinter<t>\
    {static std::string str() {return #t;}};
    
FOR_EACH_CHAR_TYPE(DEF_typeprinter)

template<typename T>
struct typeprinter<const T &> {
    static std::string str() {
        return "const " + typeprinter<T>::str() + "&";
        }
};

template<typename T, std::size_t N>
struct typeprinter<const T (&)[N]> {
    static std::string str() {
        return "const (&" + typeprinter<T>::str() + ")"
                      "[" + std::to_string(N) + "]";
    }
};

#define PT(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << typeprinter<__VA_ARGS__>::str() << std::endl;
#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
              
void show_sizeof_chartypes() {
#define GEN_show_sizeof(t)\
    std::cout << "sizeof(" #t "): " << sizeof(t) << '\n';
    FOR_EACH_CHAR_TYPE(GEN_show_sizeof)
#undef GEN_show_sizeof
}

void show_charvalues_hex() {
    PX(static_cast<long long>(L'x'))
    PX(static_cast<long long>(L'ü'))
    PX(static_cast<long long>(L'\U00010191'))
}

void show_char_literals() {
    PT(decltype('x'))
//  PT(decltype('ü'))
//  PT(decltype('\U00010191'))
    PT(decltype(L'x'))
    PT(decltype(L'ü'))
    PT(decltype(L'\U00010191'))
#if __cplusplus > 201402L
    PT(decltype(u8'x'))
#endif
//  PT(decltype(u8'ü'))
//  PT(decltype(u8'\U00010191'))
    PT(decltype(u'x'))
    PT(decltype(u'ü'))
//  PT(decltype(u'\U00010191'))
    PT(decltype(U'x'))
    PT(decltype(U'ü'))
    PT(decltype(U'\U00010191'))
}

void show_string_literals() {
    PT(decltype("x"))
    PT(decltype("ü"))
    PT(decltype("\U00010191"))
    PT(decltype(L"x"))
    PT(decltype(L"ü"))
    PT(decltype(L"\U00010191"))
    PT(decltype(u8"x"))
    PT(decltype(u8"ü"))
    PT(decltype(u8"\U00010191"))
    PT(decltype(u"x"))
    PT(decltype(u"ü"))
    PT(decltype(u"\U00010191"))
    PT(decltype(U"x"))
    PT(decltype(U"ü"))
    PT(decltype(U"\U00010191"))
}

void show_deref_string_literals() {
    PT(decltype(*"x"))
    PT(decltype(*"ü"))
    PT(decltype(*"\U00010191"))
    PT(decltype(*L"x"))
    PT(decltype(*L"ü"))
    PT(decltype(*L"\U00010191"))
    PT(decltype(*u8"x"))
    PT(decltype(*u8"ü"))
    PT(decltype(*u8"\U00010191"))
    PT(decltype(*u"x"))
    PT(decltype(*u"ü"))
    PT(decltype(*u"\U00010191"))
    PT(decltype(*U"x"))
    PT(decltype(*U"ü"))
    PT(decltype(*U"\U00010191"))
}

int main() {
    show_sizeof_chartypes();
    show_charvalues_hex();
    show_char_literals();
    show_string_literals();
    show_deref_string_literals();
}
