// http://coliru.stacked-crooked.com/a/749f61cf01481d90

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))
#define PL()\
    ((void)(std::cout << std::endl))

#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

// The following allows to switch between standard C++11 and Boost in case you
// have a broken implementation of the C++ regular expression library for Linux.
// Note that you will have to add `-lboost_regex` to the command line AND that
// on some version of GCC there is a conflict between -std=c++11 and including
// the Boost header. Omitting the command line flag is probably not the solution
// but only as the example series makes explicit use of C++11 features like
// raw string literals. Otherwise there are little problems caused by switching
// between both implementations. One that has been found is that the standard
// implementation is more tolerant in accessing a `std::smatch` object in case
// the regular expression didn't match - a situation in which the Boost throws.

#if 1
#include <regex>
namespace srx = std;
#else
#include <boost/regex.hpp>
namespace srx = boost;
#endif

namespace my {
    bool euro_parser(const std::string &str, double& ec) {
        using namespace std;
    //  using namespace boost;
    //  regex re{"([1-9][0-9]*),([0-9][0-9])"};
        regex re{"([1-9][0-9]{0,2}(?:[.][0-9]{3})*),([0-9]{2})"};
        smatch m;
        if (!regex_match(str, m, re))
            return false;
        const std::string es{m[1].str()};
        ec = std::stod(regex_replace(es, regex{"\\."}, ""))
           + std::stod(m[2].str())/100.0;
        return true;
    }
}

void re_basics() {
    double euro_cent;
    PX(my::euro_parser("1,89", euro_cent));         PX(euro_cent);
    PX(my::euro_parser("123,89", euro_cent));       PX(euro_cent);
    PX(my::euro_parser("1234567,89", euro_cent));   PX(euro_cent);
    PX(my::euro_parser("1.234.567,89", euro_cent)); PX(euro_cent);
}

namespace my {
    bool match_checker(const std::string &r, std::string s) {
        return srx::regex_match(s, srx::regex{r});
    }
}

void re_based_matching() {
    PX(my::match_checker(R"(\d+)", "1234"));
    PX(my::match_checker(R"(\d+)", "abc"));
    PX(my::match_checker(R"(\d+)", "abc1234"));
    PX(my::match_checker(R"(\d*)", "1234"));
    PX(my::match_checker(R"(\d*)", "abc1234"));
    PL();
    PX(my::match_checker(R"([-+]\d+)", "+34"));
    PX(my::match_checker(R"([-+]\d+)", "-1"));
    PX(my::match_checker(R"([-+]\d+)", "-"));
    PL();
    PX(my::match_checker(R"([-]?(\d+[.]\d*|\d*[.]\d+|\d+([.]\d*)?[eE][-+]?(\d+)))", "-12.34e-4"));
    PX(my::match_checker(R"([-]?(\d+[.]\d*|\d*[.]\d+|\d+([.]\d*)?[eE][-+]?(\d+)))", "+12.34e-4"));
    PX(my::match_checker(R"([-]?(\d+[.]\d*|\d*[.]\d+|\d+([.]\d*)?[eE][-+]?(\d+)))", "1."));
    PX(my::match_checker(R"([-]?(\d+[.]\d*|\d*[.]\d+|\d+([.]\d*)?[eE][-+]?(\d+)))", ".1"));
    PX(my::match_checker(R"([-]?(\d+[.]\d*|\d*[.]\d+|\d+([.]\d*)?[eE][-+]?(\d+)))", "123e4"));
    PX(my::match_checker(R"([-]?(\d+[.]\d*|\d*[.]\d+|\d+([.]\d*)?[eE][-+]?(\d+)))", "9e44"));
}

namespace my {
    std::string search_checker(const std::string &r, std::string s) {
        std::ostringstream os;
        os.setf(std::ios::boolalpha);
        srx::regex re{r};
        srx::smatch m;
        const bool matched = srx::regex_search(s, m, re);
        os << matched;
        // or without extra variable: os << srx::regex_search(s, m, re));
        if (true) // BUT Boost.Regex throws if m.empty() is used here!
            os << " [str=" << '"' << m.str() << '"'
               << " pos=" << m.position()
               << ", len=" << m.length() << "]";
        return os.str();
    }
}
    
void re_based_searching() {
    PX(my::search_checker(R"(\d+)", "1234"));
    PX(my::search_checker(R"(\d+)", "abc"));
    PX(my::search_checker(R"(\d+)", "abc1234"));
    PX(my::search_checker(R"(\d*)", "1234"));
    PX(my::search_checker(R"(\d*)", "abc1234"));
    PL();
    PX(my::search_checker(R"(\d*)", "12.345"));
    PX(my::search_checker(R"(<\d+>)", "abc <123> def"));
    PX(my::search_checker(R"(<.*>)", "abc <hi!> def"));
    PX(my::search_checker(R"(<.*>)", "<hi!> ... <hoh?>"));
    PX(my::search_checker(R"(<.*?>)", "<hi!> ... <hoh?>"));
    PX(my::search_checker(R"(<.*?>)", "<> ... <hoh?>"));
#if 1
    PL();
    PX(my::search_checker(R"(<.+?>)", "<!> ... <hoh?>"));
    PX(my::search_checker(R"(<.+?>)", "<> ... <hoh?>"));
    PX(my::search_checker(R"(<..*?>)", "<!> ... <hoh?>"));
    PX(my::search_checker(R"(<..*?>)", "<> ... <hoh?>"));
#endif
    PL();
    PX(my::search_checker(R"(<([^>].*?)>)", "<!> ... <hoh?>"));
    PX(my::search_checker(R"(<([^>].*?)>)", "<> ... <hoh?>"));
}

namespace my {
    std::string regex_parser(const std::string &r, std::string s,
                             bool search_if_true_match_if_false) {
        std::ostringstream os;
        os.setf(std::ios::boolalpha);
        srx::regex re{r};
        srx::smatch m;
        const auto success = search_if_true_match_if_false
                                ? srx::regex_search(s, m, re)
                                : srx::regex_match(s, m, re);
        if (success) {
            for (srx::smatch::size_type i = 0; i < m.size(); ++i)
                os << '$' << i << "=\"" << m[i].str() << "\" ";
        }
        return os.str();
    }
    
#if 0
    std::string search_parser(const std::string &r, std::string s) {
        return regex_parser(r, s, true);
    }
    std::string match_parser(const std::string &r, std::string s) {
        return regex_parser(r, s, false);
    }
#else
    #if 0
    const auto search_parser = [](const std::string r, std::string s) {
                                    return regex_parser(r, s, true);
                               };
    const auto match_parser = [](const std::string r, std::string s) {
                                    return regex_parser(r, s, false);
                               };
    #else
    using std::placeholders::_1;
    using std::placeholders::_2;
    const auto search_parser = std::bind(regex_parser, _1, _2, true);
    const auto match_parser = std::bind(regex_parser, _1, _2, false);
    #endif
#endif
}

void re_based_parsing() {
    PX(my::search_parser(R"(\d+)", "1234"));
    PX(my::search_parser(R"(\d+)", "abc1234"));
    PX(my::search_parser(R"((\d+))", "abc1234"));
    PL();
    PX(my::search_parser(R"((\d+)/(\d+))", "24/7"));
    PX(my::search_parser(R"((\d+)/(\d+))", "24/7"));
    PL();
    PX(my::match_parser (R"((\d+)/(\d+))", "a24/7z"));
    PX(my::match_parser (R"((\d+)/(\d+))", "a24/7z"));
    PX(my::search_parser(R"(^(\d+)/(\d+)$)", "a24/7z"));
    PX(my::match_parser (R"(.*(\d+)/(\d+).*)", "a24/7z"));
    PL();
    PX(my::match_parser (R"(([a-z]+)=(\d+/\d+))", "a=24/7"));
    PX(my::match_parser (R"(([a-z]+)=(\d+/\d+))", "a="));
    PX(my::match_parser (R"(([a-z]+)=(\d+/\d+)?)", "a=24/7"));
    PX(my::match_parser (R"(([a-z]+)=(\d+/\d+)?)", "a="));
    PL();
    PX(my::match_parser (R"(([a-z]+)=(\d+[.]\d*|\d*[.]\d+)/(\d+[.]\d*|\d*[.]\d+))", "a=24.5/.7"));
    PX(my::match_parser (R"(([a-z]+)=(\d+[.]\d*|\d*[.]\d+)/(\d+[.]\d*|\d*[.]\d+))", "a="));
    PL();
    PX(my::match_parser (R"(([a-z]+)=((\d+[.]\d*|\d*[.]\d+)/(\d+[.]\d*|\d*[.]\d+))?)", "a=24.5/.7"));
    PX(my::match_parser (R"(([a-z]+)=((\d+[.]\d*|\d*[.]\d+)/(\d+[.]\d*|\d*[.]\d+))?)", "a="));
    PL();
    PX(my::match_parser (R"(([a-z]+)=(?:(\d+[.]\d*|\d*[.]\d+)/(\d+[.]\d*|\d*[.]\d+))?)", "a=24.5/.7"));
    PX(my::match_parser (R"(([a-z]+)=(?:(\d+[.]\d*|\d*[.]\d+)/(\d+[.]\d*|\d*[.]\d+))?)", "a="));
}

namespace my {
    std::string regex_replacer(const std::string &r, std::string t, const std::string s) {
        return "\"" + srx::regex_replace(s, srx::regex{r}, t) + "\"";
    }
}

void re_based_replacing() {
    PX(my::regex_replacer(R"(^\s*)", "", "    abc    def      ghi    "));
    PX(my::regex_replacer(R"(\s*$)", "", "abc    def      ghi    "));
    PX(my::regex_replacer(R"(\s+)", " ", "abc    def      ghi"));
    PL();
    PX(my::regex_replacer(R"(\s+)", " ", "    abc    def      ghi    "));
    PX(my::regex_replacer(R"(^\s*(.*)\s*$)", "$1", " abc def ghi "));
    PL();
    PX(my::regex_replacer(R"(-)", "/", "2016-03-07"));
    PX(my::regex_replacer(R"((\d{4})-(\d{2})-(\d{2}))", "$2/$3/$1", "2016-03-07"));
    PX(my::regex_replacer(R"(0?(\d+)/0?(\d+)/((?:19|20)\d\d))", "$1/$2/$3", "03/07/2016"));
}

void easy_bulk_letter() {
    const std::string re{"\\s*([^;]*);\\s*([^;]*);\\s*([^;]*);?.*"};
    PX(re);
    
    const std::string letter_template{R"(
       Dear $1 $2 in $3,
       today is your lucky day...
    )"};
    PX(bulk_letter);
    
    const std::vector<std::string> address_list{
        "Mr.;  Santa Claus;  North Pole; Important Client!!",
        "Mrs.; Daisy Duck;   Duckburg;   Beauty of the town",
        "Mr.;  Mickey Mouse; Duckburg;  no comment",
        "Mr.;  Snoopy; Charlie Brown's Backyard",
    };
    for (const auto &addr : address_list) {
        PX(addr);
        PX(srx::regex_replace(addr, srx::regex{re}, bulk_letter));
    }
}

int main() {
    std::cout.setf(std::ios::boolalpha|std::ios::fixed);
    std::cout.precision(2);
    re_basics();                            PL();
//  re_based_matching();                    PL();
//  re_based_searching();                   PL();
//  re_based_parsing();                     PL();
//  re_based_replacing();                   PL();
//  easy_bulk_letter();                     PL();
}
