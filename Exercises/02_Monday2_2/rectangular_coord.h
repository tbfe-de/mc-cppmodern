#ifndef MY_RECTANGULAR_COORD
#define MY_RECTANGULAR_COORD

#include <cmath>

namespace my {

	class rectangular_coord {
	public:
		typedef double value_type;
	private:
		value_type _x;
		value_type _y;
	protected:
		rectangular_coord()
			: _x(0.0), _y(0.0)
		{}
	public:
		rectangular_coord(value_type x, value_type y)
			: _x(x), _y(y)
		{}
		value_type x() const {
			return _x;
		}
		value_type y() const {
			return _y;
		}
		void xy(value_type x, const value_type y) {
			_x = x;
			_y = y;
		}
		void x(value_type x) {
			_x = x;
		}
		void y(value_type y) {
			_y = y;
		}
	};

}

#endif //MY_RECTANGULAR_COORDINATES
