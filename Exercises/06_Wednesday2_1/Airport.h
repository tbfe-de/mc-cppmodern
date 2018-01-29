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

class Airport {
    const string name;
    list<tuple<size_t, shared_ptr<Connection>>> connections;
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
    void addConnection(shared_ptr<Connection> c, size_t n);
    void removeConnection(shared_ptr<Connection> c);
    const list<tuple<size_t, shared_ptr<Connection>>> &getConnections() const;
    static size_t instances;
};



#endif /* AIRPORT_H_ */
