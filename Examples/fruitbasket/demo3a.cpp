#include <cassert>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>

#if __cplusplus >= 201703L 
#include <any>
namespace xstd = std;
#elif __cplusplus >= 201402L
#include <experimental/any>
namespace xstd = std::experimental;
#else
#include <boost/any.hpp>
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

xstd::any random_fruit() {
    switch (std::rand() % 3) {
        case 0: std::cout << "+ adding an apple\n"; return Apple();
        case 1: std::cout << "+ adding a banana\n"; return Banana();
        case 2: std::cout << "+ adding a kiwi\n"; return Kiwi();
    }
    throw nullptr;
}

int main() {
    std::vector<xstd::any> basket;

    std::cout << "==== FILLING THE BASKET ====\n";
    for (int i = 1; i <= 10; ++i)
        basket.push_back(random_fruit());

    std::cout << "==== EMPTYING THE BASKET ====\n";
    while (!basket.empty()) {
        xstd::any piece = basket.front();
        basket.pop_back();
        try {
            auto fruit = xstd::any_cast<Apple>(piece);
            fruit.wash();
            fruit.eat();
            continue;
        }
        catch(std::bad_cast &) {}
        try {
            auto fruit = xstd::any_cast<Banana>(piece);
            fruit.peel();
            fruit.eat();
            continue;
        }
        catch(std::bad_cast &) {}
        try {
            auto fruit = xstd::any_cast<Kiwi>(piece);
            fruit.peel();
            fruit.eat();
            continue;
        }
        catch(std::bad_cast &) {}
        std::cout << "don't know how to prepare that fruit\n";
    }

}
