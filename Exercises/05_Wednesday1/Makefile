COMPILE_CPP=\
	g++ -std=c++14 -I..

all run_testrun_tests: fixarray
	@./fixarray
fixarray: fixarray.cpp
	$(COMPILE_CPP) fixarray.cpp -o $@
clean:
	rm -f a.out core *.o fixarray

