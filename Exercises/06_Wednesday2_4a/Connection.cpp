#include "Airport.h"
#include "Connection.h"

using namespace std;

size_t Connection::addAirport(shared_ptr<Airport> ap) {
	const auto result = airports.size();
	airports.emplace_back(ap, ap->getName());
	return result;
}

auto Connection::getGoingTo(size_t from) const -> vector<AirportRef> {
	vector<AirportRef> result;
	while (++from < airports.size())
		result.push_back(airports.at(from));
	return result;
}

auto Connection::getComingFrom(size_t to) const -> vector<AirportRef> {
	vector<AirportRef> result;
	for (size_t i = 0; i < to; ++i)
		result.push_back(airports.at(i));
	return result;
}

unordered_set<Connection*> Connection::instances;
