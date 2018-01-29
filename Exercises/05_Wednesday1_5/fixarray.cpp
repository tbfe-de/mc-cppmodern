#include <algorithm>
#include <memory>
#include <iterator>
#include <sstream>

namespace my {
	template<typename T>
	class fixarray {
	public:
		using value_type = T;
		using size_type = std::size_t;
	private:
		std::unique_ptr<value_type[]> arr{};
		std::size_t sz{std::size_t{0}};
		template<typename T2>
		void copy(const T2* from, std::size_t n) {
			std::copy_n(from, n, arr.get());
		}
	public:
		fixarray() =default;

		fixarray(std::size_t s)
			: arr{new value_type[s]}, sz{s}
		{}
		fixarray(const fixarray& init)
			: arr{new value_type[init.sz]}, sz{init.sz}
		{
			copy(init.arr.get(), sz);
		}
		fixarray(fixarray&& init)
			: arr{std::move(init.arr)}, sz{init.sz}
		{
			init.sz = std::size_t{0};
		}
		template<typename InIt>
		fixarray(std::size_t s, InIt from, InIt upto) 
			: arr {new value_type[s]}, sz{s} {
			auto dest = arr.get();
			for (; (s > 0) && (from != upto); --s)
				*dest++ = *from++;
			for (;(s > 0); --s)
				new (dest++) value_type{};
			
		}
		fixarray(const std::initializer_list<value_type>& init)
			: arr{new value_type[init.size()]}, sz{init.size()}
		{
				copy(init.begin(), sz);
		}
		~fixarray() =default; 
				
		fixarray& operator=(const fixarray& rhs) {
			copy(rhs.data(), std::min(sz, rhs.size()));
			return *this;
		}

		template<typename T2>
		fixarray& operator=(const fixarray<T2>& rhs) {
			copy(rhs.data(), std::min(sz, rhs.size()));
			return *this;
		}
							
		fixarray& operator=(fixarray&& rhs) {
			if (this != &rhs) {
				arr = std::move(rhs.arr);
				sz = rhs.sz;
				rhs.sz = std::size_t{0};
			}
			return *this;
		}
		
		auto size() const { return sz; }
		auto empty() const { return (sz == std::size_t{0}); }

		value_type& operator[](std::size_t idx) {
			return arr[idx];
		}
		const value_type& operator[](std::size_t idx) const {
			return arr[idx];
		}
		value_type& at(std::size_t idx) {
			if (idx >= sz)
				throw std::out_of_range("my::fixarray::at");
			return arr[idx];
		}
		const value_type& at(std::size_t idx) const {
			return const_cast<fixarray*>(this)->at(idx);
		}

		value_type* data() { return arr.get(); }
		const value_type* data() const { return arr.get(); };
		
		using iterator = value_type*;
		using const_iterator = const value_type*;
		iterator begin() { return arr.get(); }
		iterator end() { return arr.get() + sz; }
		const_iterator cbegin() const { return arr.get(); }
		const_iterator cend() const { return arr.get() + sz; } 
		const_iterator begin() const { return cbegin(); }
		const_iterator end() const { return cend(); }
	};
}

#include <string>

template<typename T>
auto to_string(const my::fixarray<T>& arg, const char* sep = "") {
    std::ostringstream s;
    for (const auto& e : arg)
        s << e << sep;
    return s.str();
}

#include "pxtn.h"

PN_(Testing Iterator Interface of `fixarray`)

int main() {
	std::cout.setf(std::ios::boolalpha);

	std::ostringstream os;
	my::fixarray<int> t{3, 15, 12, 8, 16, 7, 9, 33, 2, 3, 8, 8, 8};
	for (auto e : t)
		os << e << "; ";
	os << '\n';

	std::ostream_iterator<int> osit{os, "; "};
	std::copy_if(t.begin(), t.end(), osit,
                     [](auto e) { return !(e%2); });
	os << '\n';

	std::unique_copy(t.begin(), t.end(), osit);
	os << '\n';

	std::sort(t.begin(), t.end());
	std::unique_copy(t.begin(), t.end(), osit);
	os << '\n';
	
	std::sort(t.begin(), t.end(),
                  [](auto lhs, auto rhs) { return !(lhs < rhs); });
	std::unique_copy(t.begin(), t.end(), osit);
	os << '\n';

	PX_("3; 15; 12; 8; 16; 7; 9; 33; 2; 3; 8; 8; 8; "	"\n"
	    "12; 8; 16; 2; 8; 8; 8; "				"\n"
	    "3; 15; 12; 8; 16; 7; 9; 33; 2; 3; 8; "		"\n"
	    "2; 3; 7; 8; 9; 12; 15; 16; 33; "			"\n"
	    "33; 16; 15; 12; 9; 8; 7; 3; 2; "			"\n",
	   os.str());

#if 0 // assuming the previous tests will still run

	my::fixarray<int> x{1, 2, 3, 4, 5};
	PX_("12345", to_string(x));

	const char* const nums[] { "one", "two", "three", "four" };

	my::fixarray<const char *> y1{4, std::cbegin(nums), std::cend(nums)};
	PX_("4", y1.size());
	PX_("one; two; three; four; ", to_string(y1, "; "));
	
	my::fixarray<const char *> y2{2, std::cbegin(nums), std::cend(nums)};
	PX_("2", y2.size());
	PX_("one; two; ", to_string(y2, "; "));
	
	my::fixarray<const char *> y3{10, std::cbegin(nums), std::cend(nums)};
	PX_("10", y3.size());
	os.str(""); //std::ostringstream os;
	for (auto i = std::size_t{0}; (i < y3.size()) && y3.at(i); ++i) {
		if (i) os << ':';
		os << y3.at(i);
	}
	PX_("one:two:three:four", os.str());
 
	// default c'tor:
	my::fixarray<double> z; 	PX_("0", z.size());
					PX_("true", z.empty());
					PX_("true", !z.data());
	// usual c'tor:
	my::fixarray<double> a(3);	PX_("3", a.size());
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
	my::fixarray<int> a0(2);	PT_("my::fixarray<int>", decltype(a0));
					PX_("2", a0.size());
	a0 = b0;			PX_("2", a0.size());
					PX_("2", a0[0]);
					PX_("1", a0[1]);

	// trying with a different type
	my::fixarray<char> c(100);	PT_("char*", decltype(c.data()));
					PX_("100", c.size());
	my::fixarray<char> d{};		PT_("char*", decltype(d.data()));
					PX_("0", d.size());
	d = std::move(c);		PX_("100", d.size());
					PX_("0", c.size());

#endif
}
