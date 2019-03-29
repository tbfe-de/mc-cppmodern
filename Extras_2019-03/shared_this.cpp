#include <memory>
#include <iostream>
#include <functional>

#define PX(expr)\
    ((void)(std::cout << __FILE__  << ':' << __LINE__\
                      << "\t" #expr " --> "\
                      << (expr) << std::endl))

std::function<void(void)> callback;

class C : public std::enable_shared_from_this<C> {
	int x = 12345;
public:
	~C() { x = 0; std::cout << "destroyed @" << this << std::endl; }
	// auto self() { return std::shared_ptr<C>(this); }
	std::shared_ptr<C> self() { return shared_from_this(); }
	void greet() { std::cout << "hi, I'm alive!!" << std::endl; }
	void register_with_callback(std::function<void(void)>& cb) {
		auto me = self();
		cb = [me]() {
			PX(me.use_count());
			PX(me->x);
			me->greet();
		};
	}
};

int main() {
	{
		auto ptr = std::make_shared<C>();
		PX(ptr.use_count()); PX(ptr.get());
		auto p2 = ptr->self();
		PX(ptr.use_count()); PX(ptr.get());
		PX(p2.use_count()); PX(p2.get());
		p2->register_with_callback(callback);
		PX(p2.use_count()); PX(p2.get());
	}
	callback();
	callback = nullptr;
	std::cout << "main ENDED" << std::endl;

}

