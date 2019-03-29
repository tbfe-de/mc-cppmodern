// http://coliru.stacked-crooked.com/a/c1512f80224d37c9

#include <iostream>
#include <functional>
    
#define PX(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> " << (__VA_ARGS__) << std::endl
    
#define PT(...)\
    std::cout << __FUNCTION__ << ':' << __LINE__ << '\t'\
              << #__VA_ARGS__ " --> "\
              << typeprinter<__VA_ARGS__>::str() << std::endl
              
template<typename> struct typeprinter;

#include <string>

#define GEN_typeprinter(...)\
    template<> struct typeprinter<__VA_ARGS__>\
    {static std::string str() {return #__VA_ARGS__;}};
GEN_typeprinter(bool)
GEN_typeprinter(short)
GEN_typeprinter(int)
GEN_typeprinter(long)
GEN_typeprinter(long long)
GEN_typeprinter(unsigned short)
GEN_typeprinter(unsigned int)
GEN_typeprinter(unsigned long)
GEN_typeprinter(unsigned long long)
GEN_typeprinter(float)
GEN_typeprinter(double)
GEN_typeprinter(long double)
#undef GEN_typeprinter

#define GEN_typeprinter(sfx)\
    template<typename T> struct typeprinter<T sfx>\
    {static std::string str() {return typeprinter<T>::str() + #sfx;}};
GEN_typeprinter(*)
GEN_typeprinter(&)
GEN_typeprinter(&&)
#undef GEN_typeprinter

#define GEN_typeprinter(pfx)\
    template<typename T> struct typeprinter<pfx T>\
    {static std::string str() {return #pfx " " + typeprinter<T>::str();}};
GEN_typeprinter(const)
GEN_typeprinter(volatile)
GEN_typeprinter(const volatile)
#undef GEN_typeprinter

#include <initializer_list>
template<typename T> struct typeprinter<std::initializer_list<T>>
{static std::string str() {return "std::initializer_list<" + typeprinter<T>::str() + ">";}};

#include<array>
template<typename T, std::size_t N> struct typeprinter<std::array<T, N>>
{static std::string str() {return "std::array<" + typeprinter<T>::str() + ", " + std::to_string(N) + ">";}};
                                     
#include<vector>
template<typename T> struct typeprinter<std::vector<T>>
{static std::string str() {return "std::vector<" + typeprinter<T>::str() + ">";}};
                                     
#include<set>
template<typename T> struct typeprinter<std::set<T>>
{static std::string str() {return "std::set<" + typeprinter<T>::str() + ">";}};
                                     
#include<map>
template<typename K, typename V> struct typeprinter<std::map<K, V>>
{static std::string str() {return "std::map<" + typeprinter<K>::str() + ", " + typeprinter<V>::str() + ">";}};

void the_basics() {
//  PT(decltype({3, 2, 1, 0, -1}));
    PT(decltype(std::initializer_list<int>{3, 2, 1, 0, -1}));
    PT(decltype(std::initializer_list<float>{3.f, 2.f, 1.f, 0.f}));
    PT(decltype(std::initializer_list<double>{3, 2.5, 1, 0, -1}));
    PT(decltype(std::initializer_list<bool>{true, false, true}));
}

void stl_containers() {
    std::array<int, 5> a{{3, 2, 1, 0, -1}};
//  std::vector<int> b{std::initializer_list<int>{3, 2, 1, 0, -1}}; PX(b.size());
    std::vector<int> b{{3, 2, 1, 0, -1}};           PX(b.size());
    std::vector<int> c1{{3, 2}};                    PX(c1.size());
    std::vector<int> c2{3, 2};                      PX(c2.size());
    std::vector<int> d1{{3}};                       PX(d1.size());
    std::vector<int> d2{3};                         PX(d2.size());
    std::vector<int> d3(3);                         PX(d3.size());
    std::vector<int> d4({3});                       PX(d4.size());
    std::vector<int> e{};                           PX(e.size());
    std::set<void*> addr{&a, &b, &e};               PX(addr.size());
    const std::map<int, bool> prime{
        {1, true}, {2, true}, {3, true}, {4, false}, {5,true}
    };                                              PX(prime.size());
}

void auto_deduction() {
    auto ili = {3, 2, 1, 0, -1};        PT(decltype(ili));  PX(ili.size());
//  auto ili{3, 2, 1, 0, -1};           PT(decltype(ili));  PX(ili.size());
//  auto ili = std::initializer_list<int>{3, 2, 1, 0, -1};  PX(ili.size());

    auto ilf = {3.f, 2.f, 1.f, 0.f};    PT(decltype(ilf));  PX(ilf.size());
//  auto ilf = {3, 2.f, 1, 0, -1};      PT(decltype(ilf));  PX(ilf.size());

    auto il3 = {true, false, true};     PT(decltype(il3));  PX(il3.size());
//  auto il3{true, false, true};        PT(decltype(il3));  PX(il3.size());
    auto il2 = {true, false};           PT(decltype(il2));  PX(il2.size());
//  auto il2{true, false};              PT(decltype(il2));  PX(il2.size());
    auto il1 = {true};                  PT(decltype(il1));  PX(il1.size());
//  auto il1{true};                     PT(decltype(il1));  PX(il1.size());
//  auto il0 = {};                      PT(decltype(il0));  PX(il0.size());
    auto il0 = std::initializer_list<bool>{};               PX(il0.size());
}

#include <sstream>
std::string foo(const std::initializer_list<int> &li) {
    std::ostringstream result{};
    result.setf(std::ios::boolalpha);
    result << "initializer_list<int>{";
    for (auto it = li.begin(); it != li.end(); ++it) {
        if (it != li.begin()) result << ", ";
        result << *it;
    }
    result << "}";
    return result.str();
}

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const std::initializer_list<T>& rhs) {
    lhs << typeprinter<std::initializer_list<T>>::str() << '{';
    for (auto it = rhs.begin(); it != rhs.end(); ++it) {
        if (it != rhs.begin()) lhs << ", ";
        lhs << *it;
    }
    return lhs << '}';
}

template<typename T>
std::ostream& operator<<(std::ostream& lhs, const std::set<T>& rhs) {
    lhs << typeprinter<std::set<T>>::str() << '{';
    for (auto it = rhs.begin(); it != rhs.end(); ++it) {
        if (it != rhs.begin()) lhs << ", ";
        lhs << *it;
    }
    return lhs << '}';
}

namespace my {
//  template<typename T, template<typename ...> typename C>
    template<typename T, template<typename ...> class C>
    std::ostream& operator<<(std::ostream& lhs, const C<T>& rhs) {
        lhs << typeprinter<std::set<T>>::str() << '{';
        for (auto it = rhs.begin(); it != rhs.end(); ++it) {
            if (it != rhs.begin()) lhs << ", ";
            lhs << *it;
        }
        return lhs << '}';
    }

//  template<typename T, template<typename ...> typename C>
    template<typename T, template<typename ...> class C>
    std::string to_string(const C<T>& rhs) {
        std::ostringstream os;
    //  os << typeprinter<C<T>>::str() << '{';
        std::operator<<(os, typeprinter<C<T>>::str()) << '{';
        for (auto it = rhs.begin(); it != rhs.end(); ++it) {
            if (it != rhs.begin()) os << ", ";
                os << *it;
        }
        os << '}';
        return os.str();
    }
}

void as_argument() {
//  PX(foo(std::initializer_list<int>{3, 2, 1, 0, -1}));
    PX(foo({3, 2, 1, 0, -1}));
    
    auto li = {2, 3, 5, 7, 11, 13, 17};
    PT(decltype(li));
//  PX(my::to_string(li));
    PX(li);
    
    const std::set<int> primes{li};
    PT(decltype(primes));
    PX(my::to_string(primes));
//  PX(primes);
}
                                         
int main() {
    the_basics();
//  stl_containers();
//  auto_deduction();
//  as_argument();
}
