// http://coliru.stacked-crooked.com/a/b739113d09f09635

#include <iostream>

#define PX(x) std::cout << #x " --> " << (x) << std::endl;

#include <type_traits>

namespace my {
    // primary template
    template<typename, typename = void>
    class number;
   
#if __cplusplus < 201402L
    template<typename T>    
    class number<T, typename std::enable_if<std::is_integral<T>::value>::type> {
    public: static const char *str() { return "integral type"; }
    };
    
    template<typename T>    
    class number<T, typename std::enable_if<std::is_floating_point<T>::value>::type> {
    public: static const char *str() { return "floating type"; }
    };
#else
    template<typename T>    
    class number<T, std::enable_if_t<std::is_integral<T>{}>> {
    public: static const char *str() { return "integral type"; }
    };
    
    template<typename T>    
    class number<T, std::enable_if_t<std::is_floating_point<T>{}>> {
    public: static const char *str() { return "floating type"; }
    };
#endif
}

int main()
{
    PX(my::number<int>::str());
    PX(my::number<unsigned long>::str());
    PX(my::number<float>::str());
}
