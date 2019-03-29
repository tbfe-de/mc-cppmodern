// http://coliru.stacked-crooked.com/a/f7ed04fd0ccac524

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "<< (__VA_ARGS__)\
                      << std::endl))
namespace my {
    struct clazz {
        // acually throws under some condition
        //
        void xthrow(bool b) const
	    noexcept(false)
        {
            if (b) throw "failed to adhere to contract";
        }

        // claims never to throw
        //
        void nthrow(bool b) const
            noexcept(true)
        {
            if (b) throw "So what? I lied";
        }
    };
    struct other {
        void nthrow(bool) const;
        void xthrow(bool) const;
    };

    // claims to NEVER EVER throw - so there is no need to generate
    // book-keeping code at every point where it is called
    //
    void first (const clazz &c)
        noexcept/*(true)*/
    {
    //  c.xthrow(true);   // should not pass code review
    //  c.xthrow(false);  // should not pass code review
    //  c.nthrow(true);   // problematic, but MIGHT be ok
        c.nthrow(false);  // OK
    }

    // claims it may throw (may be it even doesn't in this version
    // but new requirements might change this, so no promise here)
    //
    void second(const clazz &c)
        noexcept(false)
    {
        c.xthrow(true);   // ok (if present)
        c.xthrow(false);  // ok (if present)
        c.nthrow(true);   // ok (if present)
        c.nthrow(false);  // ok (if present)
    }

    // This template may or may not throw, depending on its
    template<typename T>
    void third(const T &c) noexcept(   // NOTE: uncomment only ONE of the lines below
        noexcept(c.nthrow(true))            // nothrow if instantiated with clazz
    //  noexcept(c.nthrow(false))           // same (but may be misunderstood!)
    //  noexcept(c.nthrow(bool{}))          // also an option (less misleading?)
    //  noexcept(c.xthrow(true))            // NOT nothrow if instantiated with clazz
    //  noexcept(c.xthrow(false))           // same (but may be misunderstood)
    //  noexcept(c.xthrow(bool{}))          // also an option (less misleading?)
    //  noexcept(c.xthrow(bool{}) && c.nthrow(bool{}))      // can make sense
    //  noexcept(c.xthrow(bool{}) || c.nthrow(bool{}))      // possible, but ???
    ) {
    //  c.xthrow(true);   // ok (if present in nothrow specification)
    //  c.xthrow(false);  // ok (if present in nothrow specification)
        c.nthrow(true);   // ok (if present in nothrow specification)
        c.nthrow(false);  // ok (if present in nothrow specification)
    }
}

void basic_use() {
    PX(noexcept(my::clazz{}.xthrow(true)));
    PX(noexcept(my::clazz{}.xthrow(false)));
    PX(noexcept(my::clazz{}.nthrow(true)));
    PX(noexcept(my::clazz{}.nthrow(false)));

    PX(noexcept(my::other{}.xthrow(true)));
    PX(noexcept(my::other{}.xthrow(false)));
    PX(noexcept(my::other{}.nthrow(true)));
    PX(noexcept(my::other{}.nthrow(false)));

    PX(noexcept(my::first(my::clazz{})));
    PX(noexcept(my::second(my::clazz{})));
    PX(noexcept(my::third(my::clazz{})));
    PX(noexcept(my::third(my::other{})));
}

namespace my {
    template<bool B> struct use_either_or;
    template<> struct use_either_or<true>
    {static const char* call() {return "use this for true";}};
    template<> struct use_either_or<false>
    {static const char* call() {return "or that for false";}};
}

void static_select() {
    PX(my::use_either_or<true>::call());
    PX(my::use_either_or<false>::call());
    PX(my::use_either_or<noexcept(my::first(my::clazz{}))>::call());
    PX(my::use_either_or<noexcept(my::second(my::clazz{}))>::call());
    PX(my::use_either_or<noexcept(my::third(my::clazz{}))>::call());
    PX(my::use_either_or<noexcept(my::third(my::other{}))>::call());
}

namespace my {
    struct safe_move {
        const char *used;
        safe_move()                                {used = "default c'tor";}
        safe_move(const safe_move&)                {used = "copy c'tor (may throw)";} 
        safe_move(safe_move&&) noexcept            {used = "move c'tor (non-throwing)";} 
        safe_move& operator=(const safe_move&)     {used = "copy assign (may throw)"; return *this;} 
        safe_move& operator=(safe_move&&) noexcept {used = "move assign (non-throwing)"; return *this;} 
    };
    struct unsafe_move {
        const char *used;
        unsafe_move()                              {used = "default c'tor";}
        unsafe_move(const unsafe_move&)            {used = "copy c'tor (may throw)";} 
        unsafe_move(unsafe_move&&)                 {used = "move c'tor (may throw)";} 
        unsafe_move& operator=(const unsafe_move&) {used = "copy assign (may throw)"; return *this;} 
        unsafe_move& operator=(unsafe_move&&)      {used = "move assign (non-throwing)"; return *this;} 
    };
}

#include <utility>

void move_or_copy() {
    // Important: moving from the SAME object more than once is ONLY OK here
    // as the demo move operations leave their operand fully intact, which
    // is not guaranteed and AND EVEN UNTYPICAL for real world applications.

    my::safe_move s;                                      PX(s.used);
    my::safe_move s_must_move{std::move(s)};              PX(s_must_move.used);
    my::safe_move s_may_copy{std::move_if_noexcept(s)};   PX(s_may_copy.used);
    my::safe_move s2;
    PX((s2 = std::move(s)).used);
    PX((s2 = std::move_if_noexcept(s)).used);
 
    my::unsafe_move u;                                    PX(u.used);
    my::unsafe_move u_must_move{std::move(u)};            PX(u_must_move.used);
    my::unsafe_move u_may_copy{std::move_if_noexcept(u)}; PX(u_may_copy.used);
    my::unsafe_move u2;
    PX((u2 = std::move(u)).used);
    PX((u2 = std::move_if_noexcept(u)).used);
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    basic_use();
//  static_select();
//  move_or_copy();
}
