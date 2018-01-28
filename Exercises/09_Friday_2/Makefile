ALL_TESTS=\
	print_native_array

all: $(ALL_TESTS)
	@for x in $(ALL_TESTS); do ./$$x; done
print_native_array: print_native_array.cpp
	g++ -std=c++14 -O2 print_native_array.cpp -o$@
clean:
	rm -f a.out core *.o $(ALL_TESTS)

