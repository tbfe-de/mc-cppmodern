// http://coliru.stacked-crooked.com/a/1209a09c4f1a1db2

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
          
#include <chrono>

int main() {
    namespace sc = std::chrono;
#if __cplusplus >= 201402L
    using namespace std::chrono_literals;
    const sc::seconds total = 12h + 7min + 15s;
#else
    const sc::seconds total = sc::hours(12) + sc::minutes(7) + sc::seconds(15);
#endif
    PX(total.count());
}
