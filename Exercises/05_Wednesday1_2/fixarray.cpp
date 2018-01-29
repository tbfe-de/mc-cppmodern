#include <memory>

namespace my {
	template<typename T>
	class fixarray {
		std::unique_ptr<T[]> arr{};
		std::size_t sz{std::size_t{0}};
	public:
		fixarray() =default;			// may NOT be omitted
		// fixarray() {}			// (or alternatively)

		fixarray(std::size_t s)
			: arr{new T[s]}, sz{s}
		{}
						// all this may be also omitted
		fixarray(const fixarray&) 	     =delete;  // copy c'tor
		fixarray(fixarray&&) 		     =default; // move c'tor
		~fixarray() 			     =default; // d'tor
		fixarray& operator=(const fixarray&) =delete;  // copy assign
		fixarray& operator=(fixarray&&)      =default; // move assign
		
		auto size() const { return (arr.get()) ? sz : 0; }

		T& operator[](std::size_t idx) {
			return arr[idx];
		}
		const T& operator[](std::size_t idx) const {
			return arr[idx];
		}

		T* data() { return arr.get(); }
		const T* data() const { return arr.get(); };
	};
}

#include "pxtn.h"

PN_(Tests of Extended Class `fixarray`)

int main() {
	std::cout.setf(std::ios::boolalpha);

	// default c'tor:
	my::fixarray<double> z; 	PX_("0", z.size());
					PX_("true", !z.data());
	// usual c'tor:
	my::fixarray<double> a{3};	PX_("3", a.size());
					PX_("true", !!a.data());
	a[0] = 2.5;			PX_("2.5", a[0]);
					PX_("2.5", *a.data());
	a[1] = a[0]/2;			PX_("1.25", a[1]);
	a[2] = a[0] + a[1];		PX_("3.75", a[2]);
					PT_("double&", decltype(a[0]));
					PT_("double*", decltype(a.data()));

	// copy c'tor -- will NOT compile
	// auto b0 = a;

	// move c'tor			
					const auto p = a.data();
	const auto b{std::move(a)};	PX_("3", b.size());
					PX_("true", b.data() == p);
					PX_("true", a.data() == nullptr);
					PX_("2.5", b[0]);
					PX_("2.5", *b.data());
					PX_("1.25", b[1]);
					PX_("3.75", b[2]);
					PT_("double const&", decltype(b[0]));
					PT_("double const*", decltype(b.data()));

	// modifying `const` -- will NOT compile
	// *b.data() = 0.0;
	// b[1] = 0.0; 

	// copy assignment -- will NOT compile
	// a = b;

	// this looks like a move but isn't -- will NOT compile
	// a = std::move(b);

	// trying with a different type
	my::fixarray<char> c{100};	PT_("char*", decltype(c.data()));
					PX_("100", c.size());
	my::fixarray<char> d{};		PT_("char*", decltype(d.data()));
					PX_("0", d.size());
	d = std::move(c);		PX_("100", d.size());
					PX_("0", c.size());

}
