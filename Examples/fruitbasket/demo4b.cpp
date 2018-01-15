#include <cassert>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <memory>

#if __cplusplus >= 201703L 
#include <variant>
namespace std_get_if_or_boost_get = std::get_if
#else
#include <boost/variant.hpp>
namespace xstd = boost;
#define std_get_if_or_boost_get boost::get
#endif

class Apple {
public:
    const char *kind;
    Apple() : kind("an apple") {}
    void wash() { std::cout << ": wash " << kind << '\n'; }
    void eat() { std::cout << "+ eat " << kind << '\n'; }
};

class Banana {
public:
    const char *kind;
    Banana() : kind("a banana") {}
    void peel() { std::cout << "- peel " << kind << '\n'; }
    void eat() { std::cout << "+ eat " << kind << '\n'; }
};

class Kiwi {
public:
    const char *kind;
    Kiwi() : kind("a kiwi") {}
    void peel() { std::cout << "- peel " << kind << '\n'; }
    void eat() { std::cout << "+ eat " << kind << '\n'; }
};

class FruitBasket {
    std::vector<xstd::variant<Apple, Banana, Kiwi>> content;
public:
    template<typename Fruit>
    void put_in(const Fruit &fruit) {
        std::cout << "adding " << fruit.kind << '\n';
        content.emplace_back(fruit);
    }
    template<typename Fruit>
    bool take_out(Fruit &fruit) {
        assert(!empty());
        if (auto handle = std_get_if_or_boost_get<Fruit>(&content.front())) {
            fruit = *handle;
            content.pop_back();
            return true;
        }
        return false;
    }
    void throw_out_one() {
        assert(!empty());
        content.pop_back();
    }
    bool empty() const {
        return content.empty();
    }
};

int main() {
    FruitBasket basket;

    std::cout << "==== FILLING THE BASKET ====\n";
    for (int i = 1; i <= 10; ++i) {
        switch (std::rand() % 3) {
            case 0: basket.put_in(Apple()); break;
            case 1: basket.put_in(Banana()); break;
            case 2: basket.put_in(Kiwi()); break;
            default: throw nullptr;
        }
    }

    std::cout << "==== EMPTYING THE BASKET ====\n";
    while (!basket.empty()) {
        Apple an_apple;
        if (basket.take_out(an_apple)) {
            an_apple.wash();
            an_apple.eat();
            continue;
        }
        Banana a_banana;
        if (basket.take_out(a_banana)) {
            a_banana.peel();
            a_banana.eat();
            continue;
        }
        Kiwi a_kiwi;
        if (basket.take_out(a_kiwi)) {
            a_kiwi.peel();
            a_kiwi.eat();
            continue;
        }
        std::cout << "don't know how to prepare that fruit\n";
        basket.throw_out_one();
    }

}
