#pragma once
#include <string>
#include <vector>
#include <unordered_set>

template <typename string>
class Fuzzy {
    using char_type = typename string::traits_type::char_type;

public:
    Fuzzy(){};
    Fuzzy(const string& pattern);

    void set_pattern(const string& pattern);
    void set_separator(const string& separator);
    std::vector<string> search(const string& text);

private:
    std::unordered_set<char_type>       m_set;
    std::unordered_multiset<char_type>  m_multiset;
    string  m_pattern, m_separator, m_pattern_size;

    const std::vector<string> separate(const string& text);
    void make_sets(void);
};
