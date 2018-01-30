#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <memory>
    using std::shared_ptr;
#include <unordered_set>
    using std::unordered_set;
#include <string>
    using std::string;
#include <vector>
    using std::vector;

class Airport;

class Connection {
public:
private:
    const string flight;
    vector<shared_ptr<Airport>> airports;
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
    const vector<shared_ptr<Airport>> &getAirports() const {
        return airports;
    }
    size_t addAirport(shared_ptr<Airport> ap);
    vector<shared_ptr<Airport>> getComingFrom(size_t to) const;
    vector<shared_ptr<Airport>> getGoingTo(size_t from) const;
    static unordered_set<Connection*> instances;
};

#endif /* CONNECTION_H_ */
