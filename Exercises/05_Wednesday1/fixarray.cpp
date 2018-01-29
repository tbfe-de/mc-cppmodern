#include <memory>

namespace my {
	template<typename T>
	class fixarray {
		T *const arr;
		const std::size_t sz;
	public:
		fixarray(std::size_t s)
			: arr{new T[s]}, sz{s}
		{}
		fixarray(const fixarray&) =delete;
		fixarray(fixarray&& init)
			: arr{init.arr}, sz{init.sz}
		{
			init.arr = nullptr;
			init.sz = std::size_t{0};
		}
		fixarray()
			: arr{nullptr}, sz{0}
		{}
		~fixarray() {
			delete[] arr;
		}
		fixarray& operator=(const fixarray&) =delete;
		fixarray& operator=(fixarray&& rhs) {
			if (this != &rhs) {
				delete[] arr;
				arr = rhs.arr;
				sz = rhs.sz;
			}
			return *this;
		}
		auto size() const { return sz; }

		T& operator[](std::size_t idx) {
			return arr[idx];
		}
		const T& operator()(std::size_t idx) const {
			return arr[idx];
		}

		T* data() { return arr; }
		const T* data() const { return arr.get(); };
	};
}

#include "pxtn.h"

PN_(Basic Tests for Class `fixarray`)

int main() {
	my::fixarray<double> a{3};	PX_("3", a.size());
	a[0] = 2.5;			PX_("2.5", a[0]);
	a[1] = a[0]/2;			PX_("1.25", a[1]);
	a[2] = a[0] + a[1];		PX_("3.75", a[2]);
					PX_("2.5", *a.data());
}
