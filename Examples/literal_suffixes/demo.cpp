// http://coliru.stacked-crooked.com/a/1e47b87f9498ff65

#include <iostream>

#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << (__VA_ARGS__) << std::endl;
          
namespace my {
    struct length {
        const double meter;
        static length m(double v) { return length{v}; }
        static length mm(double v) { return length{v/1e3}; }
        static length km(double v) { return length{v*1e3}; }
    private:
        length(double m) : meter(m) {}
    };
    length operator+(length lhs, length rhs) {
        return length::m(lhs.meter + rhs.meter);
    }
}

my::length operator"" _m(long double parsed_value) {
    return my::length::m(parsed_value);
}
my::length operator"" _mm(long double parsed_value) {
    return my::length::mm(parsed_value);
}
my::length operator"" _km(long double parsed_value) {
    return my::length::km(parsed_value);
}
              
my::length operator"" _m(unsigned long long parsed_value) {
    return my::length::m(parsed_value);
}
my::length operator"" _mm(unsigned long long parsed_value) {
    return my::length::mm(parsed_value);
}
my::length operator"" _km(unsigned long long parsed_value) {
    return my::length::km(parsed_value);
}

int main() {
    my::length x{3.0_m};            PX(x.meter);
    my::length y{715._mm};          PX(y.meter);
    my::length z = 3_km + 120_m;    PX(z.meter);
}
