#include "Airport.h"
#include "Connection.h"

using namespace std;

void Airport::addConnection(shared_ptr<Connection> c, size_t n) {
    removeConnection(c);
    connections.emplace_back(n, c);
}

void Airport::removeConnection(shared_ptr<Connection> c) {
    connections.remove_if(
        [c](const tuple<size_t, shared_ptr<Connection>> &_1) {
            return get<1>(_1) == c;
        }
    );
}

const list<tuple<size_t, shared_ptr<Connection>>> &Airport::getConnections() const {
    return connections;
}

size_t Airport::instances;
