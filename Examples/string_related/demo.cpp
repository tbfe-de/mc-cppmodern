#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))

#include <string>
#include <cctype>
#include <algorithm>

void some_basics() {
    int i = 42; 		PX(i);
    float f{};			PX(f);
    std::string s{"hello"};	PX(s);
    PX(s = std::to_string(i));
    PX(s += ".50"); 
    PX(f = std::stof(s));
    PX(s = std::to_string(f));
    PX(f = std::stoi(s));
    PX(s.erase(s.end()-3, s.end()), s);
    PX(s.resize(s.size()-1), s);
    PX(std::stoi(s));
    PX(std::stoi("0x" + s, nullptr, 0));
}

void cvt_to_arithmetic() {
    PX(std::stoi("4250"));
    PX(std::stoi("04250", nullptr, 0));
    PX(std::stoi("0x4250", nullptr, 0));
    PX(std::stoi("4240", nullptr, 16));
    PX(std::stoi("4250", nullptr, 8));
    PX(std::stoi("4250", nullptr, 6));
    PX(std::stoi("4250", nullptr, 5));
    std::size_t vpos;
    PX(std::stoi("4250", &vpos, 5), vpos);

    PX(std::stod("1e3"));
    PX(std::stod("1e-3"));
    PX(std::stod("-1e-3"));
    PX(std::stod("-1e+3"));
    PX(std::stod("+1e+3"));
    PX(std::stod("   +1e  +3"));
    PX(std::stod("   +1e  +3", &vpos), vpos);
}

namespace my {
    std::string to_string(void *p) {
        using ptr_as_int = unsigned long long;
        return "@" + std::to_string(reinterpret_cast<ptr_as_int>(p));
    }
#if 1
    std::string to_string(std::nullptr_t) {
        return "@null";
    }
#endif
}

#include <iomanip>

void cvt_from_arithmetic() {
    PX("<" + std::to_string(6*7) + ">");
    PX("<" + std::to_string(052) + ">");
#if __cplusplus > 201402L
    PX("<" + std::to_string(0b101010) + ">");
#endif
    PX("<" + std::to_string(~0) + ">");
    PX("<" + std::to_string(~0u) + ">");

    PX("<" + std::to_string(6.7) + ">");
    PX("<" + std::to_string(.67e1) + ">");
    PX("<" + std::to_string(.123456789f) + ">");
    PX("<" + std::to_string(.123456789) + ">");
    PX("<" + std::to_string(.123456789L) + ">");
    PX("<" + std::to_string(.123456789e-6) + ">");
    PX("<" + std::to_string(.123456789e+3f) + ">");
    PX("<" + std::to_string(.123456789e+3) + ">");
    PX("<" + std::to_string(.123456789e+3L) + ">");

#if 0
    auto b = 0xCafe4U;
//  auto b = unsigned{0xCAFE4};
//  auto b = unsigned{0xC'AF'E4};
    PX(":" + std::to_string(b) + ":");
    PX(                     2*b | ~2*b       );
    PX(":" + std::to_string(2*b | ~2*b) + ":");
    PX(                     2*b || !2*b       );
    PX(":" + std::to_string(2*b || !2*b) + ":");
#endif

#if 0
    PX("~" + std::to_string(false) + "~");
    PX("~" + std::to_string(bool{}) + "~");
#endif

#if 0
//  PX("@" + std::to_string(nullptr));
//  PX("@" + std::to_string((void*)0));
//  PX("@" + std::to_string((void*)&b));
    PX("@" + std::to_string((unsigned long long)&b));
//  PX("@" + std::to_string(static_cast<unsigned long long>(&b)));
    PX("@" + std::to_string(reinterpret_cast<unsigned long long>(&b)));
    PX(my::to_string(&b));
    PX(my::to_string(nullptr));
#endif
}

void container_ops() {
    std::string s("42.5e3"); PX(s);
    auto end = std::remove_if(s.begin(), s.end(),
                              [](char c) {
                              //  return !std::isdigit(c);
                                  return !('0' <= c && c <= '9');
                              });
    s.erase(end, s.end());
    PX(s);
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    some_basics();
    cvt_from_arithmetic();
    cvt_to_arithmetic();
    container_ops();
}
