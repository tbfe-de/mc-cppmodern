all run_tests: no_dupes_copy
	@./no_dupes_copy
no_dupes_copy: no_dupes_copy.cpp
	g++ -std=c++14 no_dupes_copy.cpp -o $@
clean:
	rm -f a.out core *.o no_dupes_copy
