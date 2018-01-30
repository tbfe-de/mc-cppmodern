#ifndef AIRPORT_H_
#define AIRPORT_H_

#include <list>
    using std::list;
#include <memory>
    using std::shared_ptr;
#include <string>
    using std::string;
#include <tuple>
    using std::tuple;

class Connection;

using ConnectionRef = shared_ptr<Connection>;

class Airport {
    const string name;
    list<tuple<size_t, ConnectionRef>> connections;
public:
    Airport(const string &name_)
        : name(name_)
    {
        ++instances;
    }
    ~Airport() {
        --instances;
    }
    string getName() const {
        return name;
    }
    void addConnection(ConnectionRef c, size_t n);
    void removeConnection(ConnectionRef c);
    const list<tuple<size_t, ConnectionRef>> &getConnections() const;
    static size_t instances;
};

#endif /* AIRPORT_H_ */
