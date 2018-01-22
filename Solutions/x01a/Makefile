COMPILE_CPP = g++ -std=c++14

ALL_TESTS =\
	length_test\
	rectangular_test

all run_tests: $(ALL_TESTS)
	@for test in $(ALL_TESTS); do ./$$test; done

length_test: length.h length_test.cpp
	$(COMPILE_CPP) length_test.cpp -o $@

rectangular_test: rectangular_coord.h rectangular_test.cpp
	$(COMPILE_CPP) rectangular_test.cpp -o $@

clean:
	rm -f a.out core *.o $(ALL_TESTS)
