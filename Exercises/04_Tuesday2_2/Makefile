COMPILE_CPP=\
	g++ -std=c++14 -I..
all run_tests: slot_call
	@./slot_call
slot_call: slot_call.cpp
	$(COMPILE_CPP) slot_call.cpp -o $@
clean:
	rm -f a.out core *.o slot_call

