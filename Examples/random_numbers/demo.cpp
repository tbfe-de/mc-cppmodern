// http://coliru.stacked-crooked.com/a/b93e31046acda10f

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))

#if 1
#include <typeinfo>
#include <string>
template<typename T>
struct typeprinter {static std::string str() {return typeid(T).name();}};

#define PT(...)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << typeprinter<__VA_ARGS__>::str() << std::endl))

template<> struct typeprinter<bool> {static std::string str() {return "bool";}};
template<> struct typeprinter<char> {static std::string str() {return "char";}};
template<> struct typeprinter<int> {static std::string str() {return "int";}};
template<> struct typeprinter<long> {static std::string str() {return "long";}};
template<> struct typeprinter<unsigned long> {static std::string str() {return "unsigned long";}};
template<> struct typeprinter<long long> {static std::string str() {return "long long";}};
template<> struct typeprinter<unsigned long long> {static std::string str() {return "unsigned long long";}};
template<> struct typeprinter<float> {static std::string str() {return "float";}};
template<> struct typeprinter<double> {static std::string str() {return "double";}};
template<> struct typeprinter<std::string> {static std::string str() {return "std::string";}};
template<typename T> struct typeprinter<const T> {static std::string str() {return "const " + typeprinter<T>::str();}};
template<typename T> struct typeprinter<T*> {static std::string str() {return typeprinter<T>::str() + "*";}};
template<typename T> struct typeprinter<T&> {static std::string str() {return typeprinter<T>::str() + "&";}};
template<typename T> struct typeprinter<T&&> {static std::string str() {return typeprinter<T>::str() + "&&";}};
#else
#include <boost/type_index.hpp>
#define PT(...)\
    ((void)(std::cout << __FUNCTION__  << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << boost::typeindex::type_id_with_cvr<__VA_ARGS__>()\
                      << std::endl))
#endif

#include <cstdlib>
#include <ctime>
#include <random>

void cstyle_random() {
    PT(decltype(rand()));
    PX(RAND_MAX);
//  enable line below for "true" randomness or
//  generated sequence is identical on each run
//  std::srand(std::time(nullptr));
    for (int i = 0; i < 10; ++i)
        PX(std::rand() % 6 + 1);
}

void random_generator() {
    PT(std::mt19937::result_type);
    PX(std::mt19937::min());
    PX(std::mt19937::max());
    PT(std::mt19937_64::result_type);
    PX(std::mt19937_64::min());
    PX(std::mt19937_64::max());
    std::mt19937 gen;
    for (int n = 0; n < 10; ++n)
        PX(gen());
}
        
void randomness_check() {
//  using generator_type = std::mt19937;
    using generator_type = std::mt19937_64;
    auto gen = generator_type{};
    
    using counter_type = unsigned long long;
    enum { bit1 = (1<<7), bit2 = (1<<15) };
    auto corr_bit1_set  = counter_type{0};
    auto corr_bit2_set  = counter_type{0};
    auto corr_bit1_bit2 = counter_type{0};
    auto corr_bit1_last = counter_type{0};
    auto corr_bit2_last = counter_type{0};
    constexpr auto N = 10*1000*1000;
    PX(N);
    auto last = generator_type::result_type{0};
    for (int i = 0; i < N; ++i) {
        const auto r = gen();
        if ((bool)(r & bit1)) ++corr_bit1_set;
        if ((bool)(r & bit2)) ++corr_bit2_set;
        if ((bool)(r & bit1) == (bool)(r & bit2)) ++corr_bit1_bit2;
        if ((bool)(r & bit1) == (bool)(last & bit1)) ++corr_bit1_last;
        if ((bool)(r & bit2) == (bool)(last & bit2)) ++corr_bit2_last;
        last = r;
    }
    PX(corr_bit1_set  *1e2/N);
    PX(corr_bit2_set  *1e2/N);
    PX(corr_bit1_bit2 *1e2/N);
    PX(corr_bit1_last *1e2/N);
    PX(corr_bit2_last *1e2/N);
}

void random_distribution() {
#if 1
    // start randomly on each run
    std::random_device rd;
    std::mt19937 gen(rd());
#else
    // same sequence on each run
    std::mt19937 gen;
#endif
    std::uniform_int_distribution<> dice(1, 6);
 
    for (int n = 0; n < 10; ++n)
        PX(dice(gen));
}

#include <sstream>
#include <map>

void runlength_check() {
    std::mt19937 gen;
    std::uniform_int_distribution<> dice(1, 6);
    std::map<int, int> runlength;
    constexpr auto N = 1000;
    int last = gen();
    for (int i = 0, rl = 1; i < N; ++i) {
        const int r = dice(gen);
        if (r == last)
            ++rl;
        else
            ++runlength[rl], rl = 1, last = r;
    }
    std::ostringstream runlength_distribution;
    for (const auto &e : runlength)
        runlength_distribution << e.first << "=" << 100.*runlength[e.first]/N << "%; ";
    PX(runlength_distribution.str());
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    cstyle_random();
//  random_generator();
//  random_distribution();
//  randomness_check();
//  runlength_check();
 }
 
