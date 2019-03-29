#include <algorithm>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include "pxtn.h"

namespace my {
	template<typename T>
	class fixarray {
		std::unique_ptr<T[]> arr{};
		std::size_t sz{std::size_t{0}};
		template<typename T2>
		void copy(const T2* from, std::size_t n) {
			for (auto to = arr.get(); n > 0; --n)
				*to++ = *from++;
		}
	public:
		fixarray() =default;			// may NOT be omitted
		// fixarray() {}			// (or alternatively)

		fixarray(std::size_t s)
			: arr{new T[s]}, sz{s}
		{}
		fixarray(const fixarray& init)			// copy c'tor	
			: arr{new T[init.sz]}, sz{init.sz}
		{
			copy(init.arr.get(), sz);
		}
		fixarray(fixarray&& init) 			// move c'tor
			: arr{std::move(init.arr)}, sz{init.sz}
		{
			init.sz = std::size_t{0};
		}
		~fixarray() =default; 				// d'tor
				
		fixarray& operator=(const fixarray& rhs) {	// copy assign
			// size of fixarray ASSIGNED TO prevails
			// and element types will NOT be converted
			copy(rhs.data(), std::min(sz, rhs.size()));
			return *this;
		}

		template<typename T2>	// also copy assign, but more generic
		fixarray& operator=(const fixarray<T2>& rhs) {
			// size of fixarray ASSIGNED TO prevails
			// and element types ARE converted
			copy(rhs.data(), std::min(sz, rhs.size()));
			return *this;
		}
							
		fixarray& operator=(fixarray&& rhs) {		// move assign
			// size of fixarray ASSIGNED TO may change
			// but element types MAY NOT differ
			if (this != &rhs) {
				arr = std::move(rhs.arr);
				sz = rhs.sz;
				rhs.sz = std::size_t{0};
			}
			return *this;
		}
		
		auto size() const { return sz; }
		auto empty() const { return (sz == std::size_t{0}); }

		T& operator[](std::size_t idx) {
			return arr[idx];
		}
		const T& operator[](std::size_t idx) const {
			return arr[idx];
		}
		T& at(std::size_t idx) {
			static_assert(std::is_unsigned<decltype(idx)>::value,
				      "the test FOLLOWING requires unsigned!");
			if (idx >= sz)
				throw std::out_of_range("my::fixarray::at");
			return arr[idx];
		}
		const T& at(std::size_t idx) const {
			// the following cast is safe becausae ....
			return const_cast<fixarray*>(this)->at(idx);

		}
		T* data() { return arr.get(); }
		const T* data() const { return arr.get(); };
	};
}

PN_(Tests of Extended Class `fixarray`)

int main() {
	std::cout.setf(std::ios::boolalpha);

	// default c'tor:
	my::fixarray<double> z; 	PX_("0", z.size());
					PX_("true", z.empty());
					PX_("true", !z.data());
	// usual c'tor:
	my::fixarray<double> a{3};	PX_("3", a.size());
					PX_("false", a.empty());
					PX_("true", !!a.data());
	a[0] = 2.5;			PX_("2.5", a[0]);
					PX_("2.5", *a.data());
	a[1] = a[0]/2;			PX_("1.25", a[1]);
	a[2] = a[0] + a[1];		PX_("3.75", a[2]);
					PT_("double&", decltype(a[0]));
					PT_("double*", decltype(a.data()));

	// copy c'tor -- WILL compile now
	auto b0 = a;			PT_("my::fixarray<double>", decltype(b0));
					PX_("false", a.data() == b0.data());
					PX_("true", a.size() == b0.size());
					PX_("2.5", b0[0]);
					PX_("2.5", *b0.data());
					PX_("1.25", b0[1]);
					PX_("3.75", b0[2]);
	// move c'tor			
					const auto p = a.data();
	const auto b{std::move(a)};	PX_("3", b.size());
					PX_("true", b.data() == p);
					PX_("0", a.size());
					PX_("true", a.data() == nullptr);
					PX_("2.5", b[0]);
					PX_("2.5", *b.data());
					PX_("1.25", b[1]);
					PX_("3.75", b[2]);
					PT_("double const&", decltype(b[0]));
					PT_("double const*", decltype(b.data()));
					PX_("1.25", b[1]);

	// modifying `const` -- will NOT compile
	// *b.data() = 0.0;
	// b[1] = 0.0; 

	// this looks like a move but isn't -- WILL compile now
	a = std::move(b);		PX_("0", a.size());
					PX_("true", a.data() == nullptr);

	// still NOT move but a copy
	b0 = std::move(b); 		PX_("false", b0.data() == b.data());
					PX_("2.5", b0[0]);
					PX_("1.25", b0[1]);
					PX_("3.75", b0[2]);
					PX_("2.5", b0[1] *= 2);
					PX_("1.25", b[1]);

	// copy assignment -- WILL compile now
					const auto p2 = b0.data();
	b0 = b;				PX_("true", b0.data() == p2);
					PX_("2.5", b0[0]);
					PX_("1.25", b0[1]);
					PX_("3.75", b0[2]);

	// more generic copy (accepts assignment compatible types)
	my::fixarray<int> a0{2};	PT_("my::fixarray<int>", decltype(a0));
					PX_("2", a0.size());
	a0 = b0;			PX_("2", a0.size());
					PX_("2", a0[0]);
					PX_("1", a0[1]);

	// trying with a different type
	my::fixarray<char> c{100};	PT_("char*", decltype(c.data()));
					PX_("100", c.size());
	my::fixarray<char> d{};		PT_("char*", decltype(d.data()));
					PX_("0", d.size());
	d = std::move(c);		PX_("100", d.size());
					PX_("0", c.size());
}
