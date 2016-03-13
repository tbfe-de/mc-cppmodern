// http://coliru.stacked-crooked.com/a/b46ba3d4b3e78bd6

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;

void plain_reference_usage() {
    int x = 0;                  PX(x)
    const int cx = 101;         PX(cx)
    int &xr = x;                PX(xr)
                                PX(++xr)
                                PX(x)
    const int &cxr = cx;        PX(cxr)
//                              PX(++cxr)
    int&& z = 2*x;              PX(z)
                                PX(++z)
                                PX(x)
}

void foo(int& ri)           { PX(++ri); }
void foo(const int &cri)    { PX(cri); }
void foo(int&& rri)         { PX(rri); }
              
void reference_based_overloading() {
    int x = 0;
    const int cx = 101;
    foo(x);
    foo(cx);
    foo(2*x);
}

int main() {
    plain_reference_usage();
    reference_based_overloading();
}
