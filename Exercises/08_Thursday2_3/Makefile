ALL_TESTS=\
	performance1\
	performance2\
	performance3

all: $(ALL_TESTS)
	@echo "two huge chunks ..." && ./performance1
	@echo "few large chunks ..." && ./performance2
	@echo "many small chunks ..." && ./performance3
performance1: performance.cpp
	g++ -std=c++14 -O2 -DTWO_HUGE_CHUNKS performance.cpp -o$@
performance2: performance.cpp
	g++ -std=c++14 -O2 -DFEW_LARGE_CHUNKS performance.cpp -o$@
performance3: performance.cpp
	g++ -std=c++14 -O2 -DMANY_SMALL_CHUNKS performance.cpp -o$@
clean:
	rm -f a.out core *.o performance1 performance2

