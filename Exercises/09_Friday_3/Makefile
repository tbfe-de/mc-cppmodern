ALL_TESTS=\
	variadic_print

all: $(ALL_TESTS)
	@for x in $(ALL_TESTS); do ./$$x; done
variadic_print: variadic_print.cpp
	g++ -std=c++14 -O2 variadic_print.cpp -o$@
clean:
	rm -f a.out core *.o $(ALL_TESTS)

