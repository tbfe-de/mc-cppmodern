#ifndef AIRPORT_H_
#define AIRPORT_H_

#include <list>
    using std::list;
#include <memory>
    using std::shared_ptr;
    using std::weak_ptr;
#include <unordered_set>
    using std::unordered_set;
#include <string>
    using std::string;
#include <tuple>
    using std::tuple;

class Connection;

class Airport {
    const string name;
    list<tuple<size_t, weak_ptr<Connection>>> connections;
public:
    Airport(const string &name_)
        : name(name_)
    {
        instances.insert(this);
    }
    ~Airport() {
        instances.erase(this);
    }
    string getName() const {
        return name;
    }
    void addConnection(weak_ptr<Connection> c, size_t n);
    void removeConnection(weak_ptr<Connection> c = {});
    const list<tuple<size_t, weak_ptr<Connection>>> &getConnections() const;
    static std::unordered_set<Airport*> instances;
};

#endif /* AIRPORT_H_ */
