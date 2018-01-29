OOPT=
# OPTS+= -DUSE_WEAK_CONNECTION_REF_IN_AIRPORT
# OPTS+= -DUSE_WEAK_AIRPORT_REF_IN_CONNECTION

airtravel: airtravel.o Airport.o Connection.o
	g++ -std=c++0x airtravel.o Airport.o Connection.o -o $@
airtravel.o: airtravel.cpp
	g++ $(OPTS) -std=c++0x -c airtravel.cpp
Airport.o: Airport.cpp
	g++ $(OPTS) -std=c++0x -c Airport.cpp
Connection.o: Connection.cpp
	g++ $(OPTS) -std=c++0x -c Connection.cpp
airtravel.o Airport.o Connection.o: Airport.h
airtravel.o Airport.o Connection.o: Connection.h

clean:
	rm -f a.out core *.o airtravel
