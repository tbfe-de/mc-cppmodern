#include <iostream>
#include <string>

#define PX(...)\
    ((void)(std::cout << __FILE__ ":" << __LINE__\
                      << "\t" #__VA_ARGS__ " --> " << (__VA_ARGS__)\
                      << std::endl))

namespace my {
    class clazz {
    public:
    //  int a{1};
        int a = 1;
        int b = 2;
        int c;
        clazz() =default;
        clazz(int a_, int c_) : a{a_}, c{c_} {};
    };
}

void direct_member_init() {
    my::clazz x;          PX(x.a); PX(x.b); PX(x.c);
    my::clazz y{10, 20};  PX(x.a); PX(x.b); PX(x.c);
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    direct_member_init();
}
