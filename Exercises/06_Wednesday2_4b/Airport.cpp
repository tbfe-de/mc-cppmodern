#include "Airport.h"
#include "Connection.h"

using namespace std;

void Airport::addConnection(weak_ptr<Connection> c, size_t n) {
    removeConnection(c);
    connections.emplace_back(make_tuple(n, c));
}

void Airport::removeConnection(weak_ptr<Connection> c) {
    connections.remove_if(
        [c](const tuple<size_t, weak_ptr<Connection>> &_1) {
            const auto p = get<1>(_1).lock();
            return (!p || p == c.lock());
        }
    );
}

const list<tuple<size_t, weak_ptr<Connection>>> &Airport::getConnections() const {
    const_cast<Airport*>(this)->removeConnection();
    return connections;
}

unordered_set<Airport*> Airport::instances;
