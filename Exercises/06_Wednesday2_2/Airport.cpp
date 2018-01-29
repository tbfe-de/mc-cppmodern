#include "Airport.h"
#include "Connection.h"

using namespace std;

void Airport::addConnection(shared_ptr<Connection> c, size_t n) {
    removeConnection(c);
    connections.emplace_back(n, ConnectionRef(c));
}

void Airport::removeConnection(shared_ptr<Connection> c) {
    connections.remove_if(
        [c](const tuple<size_t, ConnectionRef> &_1) {
            #ifdef USE_WEAK_CONNECTION_REF_IN_AIRPORT
            auto cp = get<1>(_1).lock();
	    return !cp || cp.get() == c.get();
	    #else
            return get<1>(_1) == c;
            #endif
        }
    );
}

const list<tuple<size_t, Airport::ConnectionRef>> &Airport::getConnections() const {
    return connections;
}

size_t Airport::instances;
