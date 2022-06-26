#include "fuzzy.hpp"
#include <algorithm>

template class Fuzzy<std::string>;

template<typename string>
Fuzzy<string>::Fuzzy(const string &pattern)
{
    this->set_pattern(pattern);
}

template<typename string>
void Fuzzy<string>::set_pattern(const string &pattern)
{
    this->m_pattern = pattern;
    this->m_pattern_size = pattern.length();
    this->make_sets();
}

template<typename string>
void Fuzzy<string>::set_separator(const string &separator)
{
    this->m_separator = separator;
}

template<typename string>
std::vector<string> Fuzzy<string>::search(const string& text)
{
    std::vector<string> out;
    std::vector<string> separated;

    if (this->m_pattern.empty())
        return out;

    separated = this->separate(text);

    for (const auto& line : separated) {
        std::unordered_multiset<char_type> multiset_line;
        size_t hits = 0;

        for (const auto& ch : line)
            multiset_line.insert(ch);

        for (const auto& ch : this->m_set) {
            const auto& pattern_char_count = this->m_multiset.count(ch);
            const auto& line_char_count = multiset_line.count(ch);

            if (line_char_count >= pattern_char_count)
                hits++;
        }

        if (hits == this->m_set.size())
            out.push_back(line);
    }

    return out;
}

template<typename string>
const std::vector<string> Fuzzy<string>::separate(const string &text)
{
    std::vector<string> out;
    size_t pos, offset = 0;

    while ((pos = text.find(this->m_separator, offset)) != string::npos) {
        out.push_back(text.substr(offset, pos - offset));
        offset = pos + this->m_separator.length();
    }
    out.push_back(text.substr(offset));

    return out;
}

template<typename string>
void Fuzzy<string>::make_sets(void)
{
    for (const auto& ch : this->m_pattern) {
        this->m_set.insert(ch);
        this->m_multiset.insert(ch);
    }
}
