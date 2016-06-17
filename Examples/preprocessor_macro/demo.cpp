#include <cmath>
#include <iostream>

#define PX(expr)\
    ((void)(std::cout << __FILE__  << ':' << __LINE__\
                      << "\t" #expr " --> "\
                      << (expr) << std::endl))
                      
#if 0
#define PI 2*std::acos(0.0)
#define sq(x) ((x)*(x))
#else
namespace my {
    const/*expr*/ double PI = 2*std::acos(0.0);
    inline double sq(double x) { return x*x; }
}
#endif

int main() {
    using namespace my;
    PX(PI);
    PX(1/PI);
    const int v = 7;
    PX(sq(v));
    PX(sq(v+1));
    int z = 9;
    PX(++z);
    PX(100/sq(z));
    PX(sq(++z));
    PX(z);
}
