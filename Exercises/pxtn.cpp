#include "pxtn.h"

PN_(A Basic Demo of TDD ...)

int main() {
	PX_("true", true);
	std::cout.setf(std::ios::boolalpha);
	PX_("true", true);
	PT_("int", bool);
}	
