#include "pxtn.h"

#include "rectangular_coord.h"
#include "length.h"

PN_(Some Basic Tests of `my::rectangular_coord`)

#include <cmath>

namespace {
	template<typename T>
	struct rctest : my::rectangular_coord<T> {
		using my::rectangular_coord<T>::rectangular_coord;
	};
}

int main() {
	std::cout.setf(std::ios::boolalpha);
	std::cout.setf(std::ios::fixed);
	std::cout.precision(4);

	my::rectangular_coord<float> p1{0.866, 0.5};

	PX_("0.8660", p1.x());
	PX_("0.5000", p1.y());

	my::rectangular_coord<double> p2{p1};
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

	rctest<int> p3;
	PX_("0", p3.x());
	PX_("0", p3.y());

	rctest<bool> p4{true, false};
	PX_("true", p4.x());
	PX_("false", p4.y());

	using namespace my::literal_suffixes;
	my::rectangular_coord<my::length> p5{7.5_m, 150_cm};
	PX_("7.5000", p5.x().value());
	PX_("1.5000", p5.y().value());
}
