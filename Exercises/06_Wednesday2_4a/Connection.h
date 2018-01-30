#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <memory>
    using std::shared_ptr;
#include <unordered_set>
    using std::unordered_set;
#include <string>
    using std::string;
#include <tuple>
    using std::tuple;
#include <vector>
    using std::vector;

class Airport;

class Connection {
public:
    typedef tuple<weak_ptr<Airport>, string> AirportRef;
private:
    const string flight;
    vector<AirportRef> airports;
public:
    Connection(const string &flight_)
        : flight(flight_)
    {
        instances.insert(this);
    }
    ~Connection() {
        instances.erase(this);
    }
    string getFlight() const {
        return flight;
    }
    const vector<AirportRef> &getAirports() const {
        return airports;
    }
    size_t addAirport(shared_ptr<Airport> ap);
    vector<AirportRef> getComingFrom(size_t to) const;
    vector<AirportRef> getGoingTo(size_t from) const;
    static unordered_set<Connection*> instances;
};

#endif /* CONNECTION_H_ */
