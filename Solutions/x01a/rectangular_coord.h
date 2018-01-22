#ifndef MY_RECTANGULAR_COORD
#define MY_RECTANGULAR_COORD

#include <cmath>

namespace my {

	class rectangular_coord {
		using value_type = double;
	private:
		value_type _x = 0.0;
		value_type _y = 0.0;
	protected:
		rectangular_coord() {}
	public:
		rectangular_coord(value_type x, value_type y)
			: _x(x), _y(y)
		{}
		auto x() const {
			return _x;
		}
		auto y() const {
			return _y;
		}
		auto xy(value_type x, const value_type y) {
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
