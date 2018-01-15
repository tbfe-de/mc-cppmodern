#include <cassert>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>

#if __cplusplus >= 201703L 
#include <variant>
namespace xstd = std;
#else
#include <boost/variant.hpp>
namespace xstd = boost;
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

xstd::variant<Apple, Banana, Kiwi> random_fruit() {
    switch (std::rand() % 3) {
        case 0: std::cout << "+ adding an apple\n"; return Apple();
        case 1: std::cout << "+ adding a banana\n"; return Banana();
        case 2: std::cout << "+ adding a kiwi\n"; return Kiwi();
    }
    throw nullptr;
}

int main() {
    std::vector<xstd::variant<Apple, Banana, Kiwi>> basket;

    std::cout << "==== FILLING THE BASKET ====\n";
    for (int i = 1; i <= 10; ++i)
        basket.push_back(random_fruit());

    std::cout << "==== EMPTYING THE BASKET ====\n";
    while (!basket.empty()) {
        auto piece = std::move(basket.front());
        basket.pop_back();
        try {
            auto fruit = xstd::get<Apple>(piece);
            fruit.wash();
            fruit.eat();
            continue;
        }
        catch(xstd::bad_get &) {}
        try {
            auto fruit = xstd::get<Banana>(piece);
            fruit.peel();
            fruit.eat();
            continue;
        }
        catch(xstd::bad_get &) {}
        try {
            auto fruit = xstd::get<Kiwi>(piece);
            fruit.peel();
            fruit.eat();
            continue;
        }
        catch(xstd::bad_get &) {}
        std::cout << "don't know how to prepare that fruit\n";
    }

}
