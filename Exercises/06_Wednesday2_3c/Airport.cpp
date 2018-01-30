#include "Airport.h"
#include "Connection.h"

using namespace std;

void Airport::addConnection(ConnectionRef c, size_t n) {
    removeConnection(c);
    connections.emplace_back(n, c);
}

void Airport::removeConnection(ConnectionRef c) {
    connections.remove_if(
        [c](const tuple<size_t, ConnectionRef> &_1) {
            return get<1>(_1).lock() == c.lock();
        }
    );
}

const list<tuple<size_t, ConnectionRef>> &Airport::getConnections() const {
    return connections;
}

size_t Airport::instances;
