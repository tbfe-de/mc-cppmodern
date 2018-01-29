#include "Airport.h"
#include "Connection.h"

using namespace std;

size_t Connection::addAirport(AirportRef ap) {
	const auto result = airports.size();
	airports.push_back(ap);
	return result;
}

vector<Connection::AirportRef> Connection::getGoingTo(size_t from) const {
	vector<AirportRef> result;
	while (++from < airports.size())
		result.push_back(airports.at(from));
	return result;
}

vector<Connection::AirportRef> Connection::getComingFrom(size_t to) const {
	vector<AirportRef> result;
	for (size_t i = 0; i < to; ++i)
		result.push_back(airports.at(i));
	return result;
}

size_t Connection::instances;
