// http://coliru.stacked-crooked.com/a/9fcd9f0298efdf48

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
              
              
// text adjustments Left, Center, Right
enum class Adjust /*: char */ {
    L = 'l',
    C = 'c',
    R = 'r',
};
          
// colors Red, Green, Blue
enum Color : unsigned char {
    R = (1<<0),
    G = (1<<1),
    B = (1<<2),
};

int main() {
#if 0
    PX(L)
    PX(C)
    PX(R)
#else
    PX(static_cast<char>(Adjust::L))
    PX(static_cast<char>(Adjust::C))
    PX(static_cast<char>(Adjust::R))
    PX(sizeof(Adjust))
    PX(R)
    PX(G)
    PX(B)
    PX(R|G)
    PX(R|G|B)
    PX(sizeof(Adjust))
#endif
}
