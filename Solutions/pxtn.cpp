#include "pxtn.h"

int main() {
	const auto &v1 = 123;
	auto &v2 = v1;
	PX(v1);
	PX(v2);
	PX(++v2);
	PX(v1);
}	
