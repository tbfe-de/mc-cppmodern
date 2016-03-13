#if __cplusplus < 201402L
#error "This code makes use of C++14 features"
#endif

namespace my {
    class first {};
    class other : first {};
}

#include <type_traits>

int main() {
    static_assert(std::is_convertible<my::other, my::first>{}(),
                  "my::other must be convertble in my::first");
}
