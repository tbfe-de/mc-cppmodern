all: airtravel
	@echo '?' | ./airtravel && ./airtravel
airtravel: airtravel.o Airport.o Connection.o
	g++ -std=c++11 airtravel.o Airport.o Connection.o -o $@
airtravel.o: airtravel.cpp
	g++ -std=c++11 -c airtravel.cpp
Airport.o: Airport.cpp
	g++ -std=c++11 -c Airport.cpp
Connection.o: Connection.cpp
	g++ -std=c++11 -c Connection.cpp
airtravel.o Airport.o Connection.o: Airport.h
airtravel.o Airport.o Connection.o: Connection.h

clean:
	rm -f a.out core *.o airtravel
