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
        int a = c+3;
        int b = 2;
        int c = 0;
        clazz() {};
        clazz(int a_, int c_) : a{a_}, c{c_} {};
    };
}

void direct_member_init() {
    my::clazz x;          PX(x.a); PX(x.b); PX(x.c);
    my::clazz y{10, 20};  PX(y.a); PX(y.b); PX(y.c);
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    direct_member_init();
}
