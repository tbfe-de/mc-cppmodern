// http://coliru.stacked-crooked.com/a/549408ad7b6130c9

#include <iostream>
#include <boost/type_index.hpp>

#define PT(type)\
    std::cout << #type << " --> "\
              << boost::typeindex::type_id_with_cvr<type>() << std::endl;
                           
int main() {
//  PT(volatile long *const *&);
                        PT(decltype(42))
    const int ci = 42;  PT(decltype(ci))
    int i = ci;         PT(decltype(i))
                        PT(decltype(2*i))
    int &ri = i;        PT(decltype(ri))
                        PT(decltype(2*ri))
    const int *p = &i;  PT(decltype(p))
                        PT(decltype(*p))
                        PT(decltype(*p + 0))
}
