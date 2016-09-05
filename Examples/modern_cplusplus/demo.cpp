// http://coliru.stacked-crooked.com/a/8257d868776840dd

#include <iostream>
#include <functional>
    
#define PX(x)\
    do { using namespace std;\
         cout << #x " --> " << (x) << endl;\
    } while (false)
    
#if 1
#include <typeinfo>
#define PT(t)\
    do { using namespace std;\
         cout << #t " --> " << typeid(t).name() << endl;\
    } while (false)

#else
#include <boost/type_index.hpp>
#define PT(t)\
    do { using namespace std; namespace bm = boost::typeindex;\
         cout << #t " --> " << bm::type_id_with_cvr<t>() << endl;\
    } while (false)
#endif
    
using ULL = unsigned long long;
#if 1
constexpr int faculty(int n) {
    return (n == 0) ? 1 : n*faculty(n-1);
}
#else
std::function<ULL (ULL)> faculty =
    [](ULL n) -> ULL { return n ? n*faculty(n-1) : 1uLL; };
#endif

auto foo(bool tf) -> const int* {
    static constexpr int x{faculty(6)};
    return tf ? &x : nullptr;
    //if (tf) return &x; else return nullptr;
}

int main() {
    PX(faculty(6));
    PT(decltype(foo(true)));
}
