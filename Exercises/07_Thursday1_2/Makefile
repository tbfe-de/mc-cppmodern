ALL_TESTS=\
	no_dupes_copy1\
	no_dupes_copy2\
	no_dupes_copy3
	
all run_tests: $(ALL_TESTS)
	@for x in $(ALL_TESTS);\
	 do echo "$$x ..."; ./$$x;\
	 done
no_dupes_copy1: no_dupes_copy.cpp
	g++ -std=c++98 no_dupes_copy.cpp -o $@
no_dupes_copy2: no_dupes_copy.cpp
	g++ -std=c++11 no_dupes_copy.cpp -o $@
no_dupes_copy3: no_dupes_copy.cpp
	g++ -std=c++14 no_dupes_copy.cpp -o $@
clean:
	rm -f a.out core *.o $(ALL_TESTS)
