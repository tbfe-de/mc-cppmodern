// http://coliru.stacked-crooked.com/a/734c043109f45d23

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "<< (__VA_ARGS__)\
                      << std::endl))

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <set>
#include <string>

namespace my {
    struct airport;
    struct flight;
    using airport_ref = std::shared_ptr<airport>;
    using flight_ref_strong = std::shared_ptr<flight>;
    using flight_ref = std::weak_ptr<flight>;
    struct airport {
        const std::string id;
        std::vector<flight_ref> connections;
        airport(const std::string &id_)
            :id(id_)
        { ++instances; }
        void add_flight(flight_ref_strong);
        std::size_t clear_dead_connections();
        ~airport() { --instances; }
        // ...
        static std::size_t instances;
    };
    
    struct flight : std::enable_shared_from_this<flight> {
        const std::string id;
        std::vector<airport_ref> route;
        flight(const std::string &id_);
        ~flight() {
            --instances;
        }
        void set_route(std::initializer_list<airport_ref> airports);
        // ...
        static std::size_t instances;
        static unsigned long long uid;
    };

    std::size_t airport::instances{0};

    void airport::add_flight(flight_ref_strong fs) {
        (void) clear_dead_connections();
        auto known = [fs](flight_ref f) {
                      // return flight_ref_strong(f).get() == fs.get();
                         return f.lock() && f.lock().get() == fs.get();
                     };
        if (std::find_if(connections.begin(), connections.end(),
                         known) == connections.end())
            connections.push_back(fs);
    }

    std::size_t flight::instances{0};

    flight::flight(const std::string &id_)
        : id(id_) {
        ++instances;
    }
            
    void flight::set_route(std::initializer_list<airport_ref> airports) {
        route.clear();
        route.reserve(airports.size());
        for (auto &a : airports) {
            route.push_back(a);
            a->add_flight(shared_from_this());
       }
    }
    
    std::size_t airport::clear_dead_connections() {
        const auto initial_connections = connections.size();
        auto endp = std::remove_if(connections.begin(), connections.end(),
                                   [](flight_ref f) {
                                       return !f.lock();
                                   });
        connections.erase(endp, connections.end());
        return initial_connections - connections.size();
    }
    
    std::ostream& operator<<(std::ostream& lhs, const airport_ref& rhs) {
        if (!lhs) throw std::logic_error("invalid airport reference");
        int dead_connections = rhs->clear_dead_connections();
        for (const auto &c : rhs->connections)
            lhs << flight_ref_strong(c)->id << "; ";
        if (dead_connections)
            lhs << "(" << dead_connections
                       << " dead connection(s) removed)";
        return lhs;
    }

    std::ostream& operator<<(std::ostream& lhs, const flight_ref_strong& rhs) {
        if (!lhs) throw std::logic_error("invalid flight reference");
        auto next = rhs->route.begin();
        if (next == rhs->route.end())
            return lhs << rhs->id << " (not operational)";
        lhs << (*next)->id;
        if (++next == rhs->route.end())
            return lhs << " => " << rhs->route.front()->id
                                 << " (local flight)";
        while (next != rhs->route.end())
            lhs << " => " << (*next++)->id;
        return lhs;
    }

}

void airtraffic_demo() {
    {
        auto fra = std::make_shared<my::airport>("FRA");
        auto muc = std::make_shared<my::airport>("MUC");
        auto cpg = std::make_shared<my::airport>("CPG");
        PX(my::airport::instances);
        PX(fra.use_count());
        PX(muc.use_count());
        auto mw000 = std::make_shared<my::flight>("MW000");
        auto mw001 = std::make_shared<my::flight>("MW001");
        mw001->set_route({fra});
        auto mw998 = std::make_shared<my::flight>("MW998");
        mw998->set_route({cpg, fra, muc});
        auto mw999 = std::make_shared<my::flight>("MW999");
        mw999->set_route({muc, fra, cpg});
        PX(my::flight::instances);
        PX(fra.use_count());
        PX(muc.use_count());
        PX(mw999.use_count());
        PX(fra); PX(muc); PX(cpg);
        PX(mw000); PX(mw001); PX(mw998); PX(mw999);
        mw999.reset();
        PX(fra); PX(muc); PX(cpg);
        PX(mw000); PX(mw001); PX(mw998); // PX(mw999);
    }
    PX(my::airport::instances);
    PX(my::flight::instances);
}

int main() {
    airtraffic_demo();
}


