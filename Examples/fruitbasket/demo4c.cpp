// http://coliru.stacked-crooked.com/a/016b02ecaf53e63f

#include <cassert>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <memory>

#if __cplusplus >= 201703L 
#include <variant>
namespace std_or_boost = std;
#else
#include <boost/variant.hpp>
namespace std_or_boost = boost;
#define visit apply_visitor
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

#if __cplusplus >= 201703L
struct prepare_and_eat {
#else
struct prepare_and_eat : boost::static_visitor<> {
#endif
    void operator()(Apple &an_apple) const {
        an_apple.wash();
        an_apple.eat();
    }
    void operator()(Banana &a_banana) const {
        a_banana.peel();
        a_banana.eat();
    }
    void operator()(Kiwi &a_kiwi) const {
        a_kiwi.peel();
        a_kiwi.eat();
    }
};

int main() {
    std::vector<std_or_boost::variant<Apple, Banana, Kiwi>> basket;

    std::cout << "==== FILLING THE BASKET ====\n";
    for (int i = 1; i <= 10; ++i) {
        switch (std::rand() % 3) {
            case 0: std::cout << "adding an apple\n";
                    basket.push_back(Apple()); break;
            case 1: std::cout << "adding a kiwi\n";
                    basket.push_back(Banana()); break;
            case 2: std::cout << "adding a Kiwi\n";
                    basket.push_back(Kiwi()); break;
            default: throw nullptr;
        }
    }

    std::cout << "==== EMPTYING THE BASKET ====\n";
    while (!basket.empty()) {
	auto another_fruit = std::move(basket.back());
	std_or_boost::visit(prepare_and_eat{}, another_fruit);
	basket.pop_back();
    }

}
