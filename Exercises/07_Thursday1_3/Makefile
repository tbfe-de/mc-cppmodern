ALL_TESTS=\
	no_dupes_copy
	
all test: $(ALL_TESTS)
	@for x in $(ALL_TESTS); do ./$$x; done
no_dupes_copy: no_dupes_copy.cpp
	g++ -std=c++14 no_dupes_copy.cpp -o $@
clean:
	rm -f a.out core *.o $(ALL_TESTS)
