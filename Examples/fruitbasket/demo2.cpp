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
    virtual void prepare() =0;
    void eat() { std::cout << "+ eat " << me() << '\n'; }
};

class Apple : public Fruit {
    void wash() { std::cout << ": wash " << me() << '\n'; }
public:
    Apple() : Fruit("an apple") {}
    virtual void prepare() { wash(); }
};

class Banana : public Fruit {
    void peel() { std::cout << "- peel " << me() << '\n'; }
public:
    Banana() : Fruit("a banana") {}
    virtual void prepare() { peel(); }
};

class Kiwi : public Fruit {
    void peel() { std::cout << "- peel " << me() << '\n'; }
public:
    Kiwi() : Fruit("a kiwi") {}
    virtual void prepare() { peel(); }
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
        piece->prepare();
        piece->eat();
    }
}
