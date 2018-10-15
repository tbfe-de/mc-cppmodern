#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;

// `constexpr` function to find greatest common divisor
//  
constexpr unsigned gcd(unsigned m, unsigned n) {
#if 0
    return n ? gcd(n, m % n) : m;
#else
    if (n == 0)
        return m; 
    else 
        return gcd(n, m % n); 
#endif
}

#include <cmath>
const/*expr*/ double PI = 2*std::acos(0.0);
// constexpr
double circle_area(double r) { return PI*r*r; }
// constexpr
double sphere_surface(double r) { return 4*PI*r*r; }

int main() {
   const int x = 27;                PX(x)
   constexpr auto y = gcd(12, 6);   PX(y)
   constexpr auto z = gcd(x, y);    PX(z)

   double radius = 3.0; PX(radius)
                        PX(circle_area(radius))
                        PX(sphere_surface(radius))
}
