#include <cstdlib>
#include <vector>
#include <iostream>
#include <memory>

class Fruit {
    const char *const kind;
public:
    Fruit(const char *const k) : kind(k) {}
    virtual inline ~Fruit() {}
    const char *me() const { return kind; }
    void eat() { std::cout << "+ eat " << me() << '\n'; }
};

class Apple : public Fruit {
public:
    Apple() : Fruit("an apple") {}
    void wash() { std::cout << ": wash " << me() << '\n'; }
};

class Banana : public Fruit {
public:
    Banana() : Fruit("a banana") {}
    void peel() { std::cout << "- peel " << me() << '\n'; }
};

class Kiwi : public Fruit {
public:
    Kiwi() : Fruit("a kiwi") {}
    void peel() { std::cout << "- peel " << me() << '\n'; }
};

class FruitBasket {
    std::vector<std::unique_ptr<Fruit>> content;
public:
    void put_in(std::unique_ptr<Fruit> &&fruit) {
        std::cout << "adding " << fruit->me() << '\n';
        content.push_back(std::move(fruit));
    }
    std::unique_ptr<Fruit> take_out() {
        auto fruit = std::move(content.back());
        content.pop_back();
        return fruit;
    }
    bool empty() const {
        return content.empty();
    }
};

std::unique_ptr<Fruit> random_fruit() {
    switch (std::rand() % 3) {
        case 0: return std::unique_ptr<Apple>{new Apple()};
        case 1: return std::unique_ptr<Banana>(new Banana());
        case 2: return std::unique_ptr<Kiwi>(new Kiwi());
    }
    throw nullptr;
}

int main() {
    FruitBasket basket;

    std::cout << "==== FILLING THE BASKET ====\n";
    for (int i = 1; i <= 10; ++i)
        basket.put_in(random_fruit());

    std::cout << "==== EMPTYING THE BASKET ====\n";
    while (!basket.empty()) {
        auto piece = basket.take_out();
        if (auto fruit = dynamic_cast<Apple*>(piece.get())) {
            fruit->wash();
        }
        else
        if (auto fruit = dynamic_cast<Banana*>(piece.get())) {
            fruit->peel();
        }
        else
        if (auto fruit = dynamic_cast<Kiwi*>(piece.get())) {
            fruit->peel();
        }
        else {
            std::cout << "don't know how to prepare"
                      << piece->me() << '\n';
        }
        piece->eat();
    }
}
