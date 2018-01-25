#ifndef MY_RECTANGULAR_COORD
#define MY_RECTANGULAR_COORD

#include <cmath>

namespace my {

	template<typename T>
	class rectangular_coord {
	public:
		using value_type = T;
	private:
		value_type _x = 0.0;
		value_type _y = 0.0;
	protected:
		rectangular_coord() {}
	public:
		rectangular_coord(value_type x, value_type y)
			: _x{x}, _y{y}
		{}
		rectangular_coord(const rectangular_coord&) =default;
		rectangular_coord(rectangular_coord&&) =default;
		template<typename T2>
		rectangular_coord(const rectangular_coord<T2>& init)
			: _x{init.x()}, _y{init.y()}
		{}
		auto x() const {
			return _x;
		}
		auto y() const {
			return _y;
		}
		auto xy(value_type x, value_type y) {
			_x = x;
			_y = y;
		}
		auto x(value_type x) {
			_x = x;
		}
		auto y(value_type y) {
			_y = y;
		}
	};

}

#endif //MY_RECTANGULAR_COORDINATES
