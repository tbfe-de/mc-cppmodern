// http://coliru.stacked-crooked.com/a/d3f1fc362e5ed6b1

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "<< (__VA_ARGS__)\
                      << std::endl))

#include <memory>
#include <string>

namespace my {
    template<typename T>
    class link_elem {
    public:
        using data_t = T;
        using next_t = std::shared_ptr<link_elem>;
    private:
        data_t data_{};
        next_t next_{};
    public:
        static std::size_t instances;
        link_elem() {++instances;}
        ~link_elem() {--instances;}
        auto data() -> data_t const {return data_;}
        auto data(const data_t &data) -> data_t {return data_ = data;}
        auto next() -> next_t const { return next_; }
        auto next(const next_t &next) -> next_t {return next_ = next;}
    };
    template<typename T>
    std::size_t link_elem<T>::instances{0};

    class clazz {
        std::shared_ptr<clazz> next_{};
    public:
        static std::size_t instances;
        clazz() {++instances;}
        ~clazz() {--instances;}
        void next(const std::shared_ptr<clazz> &next) {next_ = next;}
    };
    std::size_t clazz::instances{0};
}

#include <vector>

void circular_demo() {
#if 1
    using node_t = my::link_elem<int>;
    using node_ref = std::shared_ptr<node_t>;
    {
    std::vector<node_ref> chain;
        {
            node_ref last{};
            PX(node_t::instances);
    	    while (chain.size() < 1000) {
            	chain.push_back(std::make_shared<node_t>());
                chain.back()->next(last);
                last = chain.back();
            }
            PX(node_t::instances);
        }
        PX(node_t::instances);
    }
    PX(node_t::instances);
#endif

    std::vector<std::shared_ptr<my::clazz>> chain;
    PX(my::clazz::instances);
    while (chain.size() < 1000) {
        chain.push_back(std::make_shared<my::clazz>());
        chain.back()->next(chain.back());
//      chain.back()->next(chain.front());
    }
    PX(my::clazz::instances);
    chain.clear();
    PX(my::clazz::instances);
}

#include <algorithm>
#include <set>
#include <initializer_list>

namespace my {
    struct airport;
    struct flight;
    using airport_ref = std::shared_ptr<airport>;
    using flight_ref = std::shared_ptr<flight>;
    struct airport {
        const std::string id;
        std::set<flight_ref> connections;
        airport(const std::string &id_)
            :id(id_)
        { ++instances; }
        void add_flight(flight_ref);
        void rm_flight(flight_ref);
        ~airport() { --instances; }
        // ...
        static std::size_t instances;
    };
    
    struct flight : std::enable_shared_from_this<flight> {
        const std::string id;
        std::vector<airport_ref> route;
        flight(const std::string &id_);
        ~flight() {
        #if 0
            for (const auto &stop : route)
                stop->rm_flight(shared_from_this());
        #endif
            --instances;
        }
        void set_route(std::initializer_list<airport_ref> airports);
        // ...
        static std::size_t instances;
    };
    bool operator<(flight_ref lhs, flight_ref rhs) {
        return (lhs.get() < rhs.get());
    }

    std::size_t airport::instances{0};

    void airport::add_flight(flight_ref f) {
        connections.insert(f);
    }
    void airport::rm_flight(flight_ref f) {
        connections.erase(f);
    }

    std::size_t flight::instances{0};

    flight::flight(const std::string &id_)
        : id(id_) {
        ++instances;
    }
            
    std::ostream& operator<<(std::ostream& lhs, const airport_ref& rhs) {
        for (const auto &c : rhs->connections)
            lhs << c->id << "; ";
        return lhs;
    }

    void flight::set_route(std::initializer_list<airport_ref> airports) {
        route.clear();
        route.reserve(airports.size());
        for (auto &a : airports) {
            route.push_back(a);
            a->add_flight(shared_from_this());
       }
    }
    
    std::ostream& operator<<(std::ostream& lhs, const flight_ref& rhs) {
        auto next = rhs->route.begin();
        if (next == rhs->route.end())
            return lhs << "(not operational)";
        lhs << (*next)->id;
        if (++next == rhs->route.end())
            return lhs << " -> " << rhs->route.front()->id << " (local)";
        while (next != rhs->route.end())
            lhs << " -> " << (*next++)->id;
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
#if 0
        for (const auto& flight : {mw000, mw001, mw998, mw999}) {
            for (const auto& airport : flight->route)
		airport->rm_flight(flight);
        }
#endif
    }
    PX(my::airport::instances);
    PX(my::flight::instances);
}

int main() {
    circular_demo();
//  airtraffic_demo();
}
