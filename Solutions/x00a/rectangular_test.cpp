#include "pxtn.h"

#include "rectangular_coord.h"

PN_(Some Basic Tests of `my::rectangular_coord`)

#include <cmath>

int main() {
	std::cout.setf(std::ios::boolalpha);
	std::cout.setf(std::ios::fixed);
	std::cout.precision(4);

	my::rectangular_coord p1{0.866, 0.5};

	PX_("0.8660", p1.x());
	PX_("0.5000", p1.y());

	my::rectangular_coord p2{p1};
	PX_("0.8660", p2.x());
	PX_("0.5000", p2.y());

	p1.x(0.5);
	PX_("0.5000", p1.x());
	PX_("0.5000", p1.y());

	p1.xy(1.0, 1.0);
	PX_("1.0000", p1.x());
	PX_("1.0000", p1.y());

	p1.y(0.0);
	PX_("1.0000", p1.x());
	PX_("0.0000", p1.y());

	class rctest : public my::rectangular_coord {
	public:
		using my::rectangular_coord::rectangular_coord;
	};
	rctest p3;
	PX_("0.0000", p3.x());
	PX_("0.0000", p3.y());
}
