#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>

using namespace std;

#include "Airport.h"
#include "Connection.h"

static const char *const help = R"(
Use one of the following         (which means)
SETUP -------------------------------------------------------
  + airport ...                  (add given airport(s))
  - airport ...                  (remove given airport(s))
  : flight [airport airport ...] (define connection)
QUERY -------------------------------------------------------
  < [select] (list outgoing flights from matching airport(s))
  > [select] (list incomming flights to matching airport(s))
  = [select] (list route of matching flight(s))
OTHER -------------------------------------------------------
  $ filename (read commands from filename)
  ?          (show this help page)
  !          (check memory usage)
  .          (terminate)
)";

class ParseException :
    virtual public runtime_error {
public:
    ParseException(const string& reason)
        : runtime_error("ERROR " + reason)
    {}
};

bool parse(const string &line, bool incf = false) {
    static map<string, shared_ptr<Connection>>knownConnections;
    static map<string, shared_ptr<Airport>> knownAirports;
    istringstream is(line + " ");
    char cmdc;
    if (!(is >> cmdc).good())
            return false;
    if (incf)
        cout << line;
    string airportName;
    string flightNumber;
    string includeFile;
    switch (cmdc) {
    case '+':
        while (is >> airportName) {
            if (knownAirports.count(airportName)) {
                cout << airportName << " already in database" << endl;
                return true;
            }
            knownAirports.insert(make_pair(airportName, make_shared<Airport>(airportName)));
            cout << "added airport " << airportName
                 << " to database" << endl;
        }
        return true;
    case '-':
        while (is >> airportName) {
            knownAirports.erase(airportName);
            cout << "removed airport " << airportName
                 << " from database" << endl;
        }
        return true;
    case ':':
        if (is >> flightNumber) {
            const auto existingConnection = knownConnections.find(flightNumber);
            const bool isKnownConnection = (existingConnection != knownConnections.end());
            if (isKnownConnection) {
                for (const auto e : knownAirports) {
                    e.second->removeConnection(existingConnection->second);
                }
                knownConnections.erase(flightNumber);
            }
            vector<shared_ptr<Airport>> visitedAirports;
            while (is >> airportName) {
                auto foundAirport = knownAirports.find(airportName);
                if (foundAirport == knownAirports.end()) {
                    throw ParseException("no such airport: " + airportName);
                }
                if (find(visitedAirports.begin(), visitedAirports.end(), foundAirport->second)
		    != visitedAirports.end()) {
                    throw ParseException("duplicate airport:" + airportName);
                    return true;
                }
                visitedAirports.push_back(foundAirport->second);
            }
            if (visitedAirports.size() < 2) {
                knownConnections.erase(flightNumber);
                if (!visitedAirports.empty())
                    throw ParseException("too few airports:" + visitedAirports.front()->getName());
                if (isKnownConnection) {
                    cout << "removed connection " << flightNumber
                         << " from database" << endl;
                }
            }
            else {
                const auto currentConnection = make_shared<Connection>(flightNumber);
                knownConnections.insert(make_pair(flightNumber, currentConnection));
                cout << (isKnownConnection ? "modifying" : "creating")
                     << " connection " << flightNumber
                     << " in database" << endl;
                for (const auto currentAirport : visitedAirports) {
                    const auto currentConnection = knownConnections[flightNumber];
                    const auto nthStop = currentConnection->addAirport(currentAirport);
                    currentAirport->addConnection(currentConnection, nthStop);
                    cout << "added airport " << currentAirport->getName()
                         << " to connection " << currentConnection->getFlight() << endl;
                }
            }
        }
        return true;
    case '<':
    case '>':
        if (!(is >> airportName >> ws).eof())
            airportName.clear();
        for (const auto e : knownAirports) {
            if (airportName != "*" && e.first.find(airportName) == string::npos)
                continue;
            cout << e.first << endl;
            for (const auto c : e.second->getConnections()) {
                const auto &li = (cmdc == '<')
                                    ? get<1>(c)->getComingFrom(get<0>(c))
                                    : get<1>(c)->getGoingTo(get<0>(c))
                                    ;
                if (!li.empty()) {
                    cout << ' ' << cmdc << ' ' << get<1>(c)->getFlight() << ':';
                    for (const auto a : li) {
                       	cout << ' ' << get<1>(a);
                    }
                    cout << endl;
                }
            }
        }
        return true;
    case '=':
        if (!(is >> flightNumber >> ws).eof())
            flightNumber.clear();
        for (const auto e : knownConnections) {
            if (flightNumber != "*" && e.first.find(flightNumber) == string::npos)
                continue;
            cout << e.first << " =";
            for (const auto a : e.second->getAirports()) {
                cout << ' ' << get<1>(a);
                if (!get<0>(a).lock())
                    cout << "[closed]";
            }
            cout << endl;
        }
        return true;
    case '!':
        cout << Airport::instances.size() << " airport(s) [";
	for(const auto &airport : Airport::instances)
	     cout << ' ' << airport->getName();
	cout << " ]" << endl;
        cout << Connection::instances.size() << " flight(s) [";
	for (const auto &connection : Connection::instances)
	     cout << ' ' << connection->getFlight();
	cout << " ]" << endl;
        return true;
    case '$':
        if (!(is >> includeFile))
            throw ParseException("file name missing");
        else {
            ifstream ifs(includeFile);
            if (!ifs.good())
                throw ParseException("cannot open file: " + includeFile);
            string line;
            while (getline(ifs, line))
                parse(line, true);
        }
        return true;
    default:
        cout << "unknown command: " << line << endl;
        // FALLTHROUGH
    case '?':
        if (incf)
            return false;
        cout << help << endl;
        return true;
    case '.':
        if (incf)
            return true;
        cout << "bye, bye" << endl;
        return false;
    }
}

int main() {
    string line;
    while (getline(cin, line)) {
        try {
            if (!parse(line))
                break;
        }
        catch (const ParseException &ex) {
            cout << ex.what() << endl;
        }
    }
}
