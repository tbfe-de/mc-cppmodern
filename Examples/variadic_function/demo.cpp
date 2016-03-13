// http://coliru.stacked-crooked.com/a/28201c30df7e03d1

#include <iostream>

#define PX(...)\
    std::cout << #__VA_ARGS__ " --> " << (__VA_ARGS__)\
              << std::endl;

#include <cstdarg>

namespace my {
    // NOTE: variable argument list must be terminated with `0uLL`
    //
    unsigned long long sum_ints(unsigned long long first, ...) {
        using namespace std;
        unsigned long long result = first;
        va_list ap;
        va_start(ap, first);
        while (auto v = va_arg(ap, unsigned long long))
            result += v;
        va_end(ap);
        return result;
    }
}

void classic_varargs() {
    PX(my::sum_ints(6uLL, 7uLL, (long long)(6 * 7), 0uLL));
//  PX(my::sum_ints(6,    7,               (6 * 7), 0uLL));
//  PX(my::sum_ints(6,    7.0,             (6 * 7), 0uLL));
//  PX(my::sum_ints(6,    7,               (6 * 7), 0));
//  PX(my::sum_ints(6,    7,               (6 * 7)));
}
   

namespace my {
    void stream_all(std::ostream &os) {}
    template<typename T, typename... Ts>
    void stream_all(std::ostream &os, const T& arg, const Ts&... args) {
        os << arg;
        if (sizeof...(args)) {
            os << ", ";
            stream_all(os, args...);
        }
    }
}

#include <sstream>
void all_to_stream() {
    std::ostringstream os;
    PX(my::stream_all(os, 6.7, 6, 7, 6*7), os.str());
}
  
#include <string>
#include <type_traits>

namespace my {
#if __cplusplus < 201402L
    auto to_string(char c) -> std::string {
        return std::string("'") + c + std::string("'");
    }
    auto to_string(const char *s) -> std::string {
        return std::string("\"") + s + std::string("\"");
    }
    auto to_string(bool b) -> std::string {
        return b ? "true" : "false";
    }
    auto to_string(std::nullptr_t) -> std::string {
        return "nullptr";
    }
    template<typename T>
    auto to_string(T&& arg) -> decltype(std::to_string(arg)) {
        return std::to_string(std::forward<T>(arg));
    }
    template<typename T>
    auto to_string(const T* arg) -> std::string {
        return "@" + std::to_string(reinterpret_cast<unsigned long long>(arg));
    }
    auto cat_to_string(const std::string& sep) -> std::string {
        return sep;
    }
    template<typename T, typename... Ts>
    auto cat_to_string(const std::string& sep, T&& arg, Ts&&... args) -> std::string {
        return to_string(std::forward<T>(arg))
             + (sizeof...(args) ? sep + cat_to_string(sep, args...) : "");
    }
#else
    using namespace std::literals::string_literals;
    auto string(char c) {
        return "\'"s + c + "\'"s;
    }
    auto string(const char *s) {
        return "\""s + s + "\""s;
    }
    auto to_string(bool b) {
        return b ? "true" : "false";
    }
    auto to_string(std::nullptr_t) {
        return "nullptr";
    }
    template<typename T>
    auto to_string(T&& arg) -> decltype(std::to_string(arg)) {
        return std::to_string(std::forward<T>(arg));
    }
    template<typename T>
    auto to_string(const T* arg) {
        return "@" + std::to_string(reinterpret_cast<unsigned long long>(arg));
    }
    auto cat_to_string(const std::string& sep) {
        return sep;
    }
    template<typename T, typename... Ts>
    auto cat_to_string(const std::string& sep, T&& arg, Ts&&... args) {
        return to_string(std::forward<T>(arg))
             + (sizeof...(args) ? sep + cat_to_string(sep, args...) : "");
    }
#endif
}

void all_concatenated() {
    PX(my::cat_to_string(" ? ", 6, 7, 6 * 7));
    PX(my::cat_to_string(" + ", 3.14, false, 6*7, "hello, world", 'z', nullptr, &std::cout));
}

int main() {
    classic_varargs();
    all_to_stream();
    all_concatenated();
}

