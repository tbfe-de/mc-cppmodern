#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <memory>
    using std::shared_ptr;
#include <string>
    using std::string;
#include <vector>
    using std::vector;

class Airport;

class Connection {
public:
    #ifdef USE_WEAK_AIRPORT_REF_IN_CONNECTION
    using AirportRef = weak_ptr<Airport>;
    #else
    using AirportRef = shared_ptr<Airport>;
    #endif
    const string flight;
    vector<AirportRef> airports;
public:
    Connection(const string &flight_)
        : flight(flight_)
    {
        ++instances;
    }
    ~Connection() {
        --instances;
    }
    string getFlight() const {
        return flight;
    }
    const vector<AirportRef> &getAirports() const {
        return airports;
    }
    size_t addAirport(AirportRef ap);
    vector<AirportRef> getComingFrom(size_t to) const;
    vector<AirportRef> getGoingTo(size_t from) const;
    static size_t instances;
};

#endif /* CONNECTION_H_ */
