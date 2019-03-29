ALL_TESTS=\
	performance-tree-1\
	performance-tree-4\
	performance-tree-9\
	performance-hash-1\
	performance-hash-4\
	performance-hash-9\
	performance-vector-1\
	performance-vector-4\
	performance-vector-9
all run_tests: $(ALL_TESTS)
	@for t in $(ALL_TESTS);\
	 do echo "$$t ..." && ./$$t;\
	 done
performance-tree-1: performance.cpp
	g++ -std=c++14 -O2 -DUSE_TREE_SET -DDUPES_RATE=1 performance.cpp -o $@
performance-tree-4: performance.cpp
	g++ -std=c++14 -O2 -DUSE_TREE_SET -DDUPES_RATE=4 performance.cpp -o $@
performance-tree-9: performance.cpp
	g++ -std=c++14 -O2 -DUSE_TREE_SET -DDUPES_RATE=9 performance.cpp -o $@
performance-hash-1: performance.cpp
	g++ -std=c++14 -O2 -DUSE_HASH_SET -DDUPES_RATE=1 performance.cpp -o $@
performance-hash-4: performance.cpp
	g++ -std=c++14 -O2 -DUSE_HASH_SET -DDUPES_RATE=4 performance.cpp -o $@
performance-hash-9: performance.cpp
	g++ -std=c++14 -O2 -DUSE_HASH_SET -DDUPES_RATE=9 performance.cpp -o $@
performance-vector-1: performance.cpp
	g++ -std=c++14 -O2 -DUSE_VECTOR -DDUPES_RATE=1 performance.cpp -o $@
performance-vector-4: performance.cpp
	g++ -std=c++14 -O2 -DUSE_VECTOR -DDUPES_RATE=4 performance.cpp -o $@
performance-vector-9: performance.cpp
	g++ -std=c++14 -O2 -DUSE_VECTOR -DDUPES_RATE=9 performance.cpp -o $@

clean:
	rm -f a.out core *.o $(ALL_TESTS)
