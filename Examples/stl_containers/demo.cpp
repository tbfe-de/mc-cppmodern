// http://coliru.stacked-crooked.com/a/997f8aa290590323

#include <iostream>

#define PX(...)\
    ((void)(std::cout << __FUNCTION__ << ':' << __LINE__\
                      << "\t" #__VA_ARGS__ " --> "\
                      << (__VA_ARGS__) << std::endl))

#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace my {
    struct clazz {
    clazz(int, std::string) {}
       #if 0
        clazz(const clazz&) =delete;
        clazz(clazz&&)      =default;
       #else
        clazz(const clazz&) =default;
        clazz(clazz&&)      =delete;
       #endif
    };
}

void emplace_vs_push() {
     std::vector<my::clazz> v;
//   v.push_back(my::clazz(42, "hi!"));
     v.emplace_back(42, "hi!");
}

void words_to_positions() {
    std::istringstream test_input(
        "Wenn hinter Fliegen Fliegen fliegen"
        " fliegen Fliegen Fliegen nach."
    );
    std::map<std::string, std::vector<int>> words;
//  std::unordered_map<std::string, std::vector<int>> words;
    using isit = std::istream_iterator<std::string>;
    int pos = 0;
    std::for_each(isit{test_input}, isit{},
                  [&words, &pos](const std::string &s) {
                      words[s].push_back(++pos);
                  });
    std::ostringstream word_positions;
    for (const auto &e : words) {
        word_positions << e.first << ':';
        for (const auto &w : e.second)
            word_positions << ' ' << w;
        word_positions << "; ";
    }
    PX(word_positions.str());
}

#include <iomanip>

void positions_to_words() {
    std::istringstream test_input(
        "Fliegen: 3 4 7 8; Wenn: 1;"
        " fliegen: 5 6; hinter: 2; nach.: 9;"
    );
//  std::set<std::string> words;
    std::unordered_set<std::string> words;
    using word_ref = decltype(words)::const_iterator;
    std::map<int, word_ref> positions;
//  std::unordered_map<int, word_ref> positions;
    std::string groupstring;
    while (std::getline(test_input, groupstring, ';')) {
        std::istringstream groupstream{groupstring};
        std::string word;
        groupstream >> std::ws;
        if (std::getline(groupstream, word, ':')) {
            auto w = words.insert(word).first;
            int p;
            while (groupstream >> p)
                positions.insert(std::make_pair(p, w));
        }
    }
    std::ostringstream text_line;
    for (const auto &e : positions)
        text_line << *e.second << ' ';
    PX(text_line.str());
}

int main() {
    emplace_vs_push();
//  words_to_positions();
//  positions_to_words();
}
