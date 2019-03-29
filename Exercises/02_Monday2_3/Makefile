COMPILE_CPP=\
	g++ -std=c++14 -I..

ALL_TESTS =\
	length_test\
	rectangular_coord_test

all run_tests: $(ALL_TESTS)
	@for test in $(ALL_TESTS); do ./$$test; done

length_test: length.h length_test.cpp
	$(COMPILE_CPP) length_test.cpp -o $@

rectangular_coord_test: rectangular_coord.h rectangular_coord_test.cpp
	$(COMPILE_CPP) rectangular_coord_test.cpp -o $@

clean:
	rm -f a.out core *.o $(ALL_TESTS)
