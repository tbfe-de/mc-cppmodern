#include "pxtn.h"

PN_(Some Basic Tests of `my::length`)

#include "length.h"

#include <cmath>

int main() {
	std::cout.setf(std::ios::fixed);
	std::cout.precision(4);
	constexpr auto PI = 2*std::acos(0.0);

	auto one_meter = my::length::m(1.0);
	auto one_inch = my::length::cm(2.54);
	auto earth_radius = my::length::km(6387);
	auto equator_length = 2*PI*earth_radius;
	
	PX_("1.0000", one_meter.value());
	PX_("0.0254", one_inch.value());

	std::cout.precision(0);
	PX_("6387000", earth_radius.value());
	PX_("40130705", equator_length.value());

	std::cout.precision(2);
	PX_("0.16", ((equator_length + one_meter)/(2*PI) - earth_radius).value());

	std::cout.precision(4);
	PX_("0.9746", (one_meter - one_inch).value());

	using my::literal_suffixes::operator"" _m;
	using my::literal_suffixes::operator"" _inch;
	PX_("0.9746", (1_m - 1_inch).value());
}
