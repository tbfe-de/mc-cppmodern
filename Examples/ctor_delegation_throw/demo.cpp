#include <iostream>

namespace my {
    struct clazz {
        clazz(bool b1) try {
            if (b1) throw "exception in clazz(bool) c'tor body\n";
            std::cout << "clazz(bool) c'tor completed\n"; 
        } catch(const char *ex) {
            std::cout << "caught in clazz(bool) c'tor: " << ex;
        };
        clazz(bool b1, bool b2) try : clazz(b1) {
            if (b2) throw "exception in clazz(bool, bool) c'tor body\n";
            std::cout << "clazz(bool, bool) c'tor completed\n"; 
        } catch(const char *ex) {
            std::cout << "caught in clazz(bool, bool) c'tor: " << ex;
        }
        ~clazz() {
            std::cout << "clazz d'tor executed\n";
        }
    };
}

void ctor_delegation_throw() {
    try {
    // ------------------------------- NO exceptions thrown
    //  my::clazz obj(false);
        my::clazz obj(false, false);
    // ------------------------------- exception thrown in ...
    //  my::clazz obj(true);        // ... direct call 
    //  my::clazz obj(false, true); // ... delegating call
    //  my::clazz obj(true, false); // ... delegated-to call
        std::cout << "clazz object ready\n";
    } catch (const char *ex) {
        std::cout << "caught in test context: " << ex;
    }
}

int main() {
    std::cout.setf(std::ios::boolalpha);
    ctor_delegation_throw();
}

