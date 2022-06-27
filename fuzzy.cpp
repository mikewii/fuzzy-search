#include "fuzzy.hpp"
#include <algorithm>

template<typename string>
Fuzzy<string>::Fuzzy()
    : m_ignore_case(false)
{
}

template<typename string>
Fuzzy<string>::Fuzzy(const string &pattern)
    : m_ignore_case(false)
{
    this->set_pattern(pattern);
}

template<typename string>
Fuzzy<string>::Fuzzy(const string &pattern, const string &separator)
    : m_separator(separator), m_ignore_case(false)
{
    this->set_pattern(pattern);
}

template<typename string>
void Fuzzy<string>::set_pattern(const string &pattern)
{
    this->m_pattern = pattern;

    if (this->m_ignore_case)
        this->to_lower(this->m_pattern);

    this->initialize_sets();
}

template<typename string>
void Fuzzy<string>::set_separator(const string &separator)
{
    this->m_separator = separator;
}

template<typename string>
void Fuzzy<string>::set_data(const string& data)
{
    this->separate(data);

    this->m_result.reserve(this->m_data.size());
}

template<typename string>
void Fuzzy<string>::set_data(const std::vector<string> &data)
{
    this->m_data = data;

    this->m_result.reserve(this->m_data.size());
}

template<typename string>
void Fuzzy<string>::process(void)
{
    this->m_result.clear();

    if (this->m_pattern.empty()) {
        this->m_result.push_back(this->convert("Fuzzy:: search pattern is empty!"));
        return;
    }

    for (auto it = this->m_data.begin(); it < this->m_data.end(); it++) {
        size_t hits = 0;
        string line = *it;

        if (this->m_ignore_case)
            this->to_lower(line);

        switch (this->m_mode) {
        default:
        case FZ_SEARCH_BY_CHAR_COUNT:
            hits = this->search_by_char_count(line);
            break;
        case FZ_SEARCH_BY_CHAR_PRESENCE:
            hits = this->search_by_char_presence(line);
            break;
        }

        if (hits == this->m_set.size())
            this->m_result.push_back(*it);
    }
}

template<typename string>
void Fuzzy<string>::set_ignore_case(const bool value)
{
    this->m_ignore_case = value;

    // reinitialize pattern to apply ignore case
    this->set_pattern(this->m_pattern);
}

template<typename string>
void Fuzzy<string>::separate(const string &text)
{
    size_t pos, offset = 0;

    while ((pos = text.find(this->m_separator, offset)) != string::npos) {
        this->m_data.push_back(text.substr(offset, pos - offset));
        offset = pos + this->m_separator.length();
    }
    this->m_data.push_back(text.substr(offset));
}

template<typename string>
void Fuzzy<string>::initialize_sets(void)
{
    this->m_set.clear();
    this->m_multiset.clear();

    for (const auto& ch : this->m_pattern) {
        this->m_set.insert(ch);
        this->m_multiset.insert(ch);
    }
}

template<typename string>
void Fuzzy<string>::to_lower(string &str)
{
    std::transform(str.begin(), str.end(), str.begin(),
    [](const char_type c)
    {
        return std::tolower(c);
    });
}

template<typename string>
const size_t Fuzzy<string>::search_by_char_count(const string &line) const
{
    std::unordered_multiset<char_type> line_multiset;
    size_t hits = 0;

    for (const auto& ch : line)
        line_multiset.insert(ch);

    for (const auto& ch : this->m_set) {
        const auto& pattern_char_count = this->m_multiset.count(ch);
        const auto& line_char_count = line_multiset.count(ch);

        if (line_char_count >= pattern_char_count)
            hits++;
    }

    return hits;
}

template<typename string>
const size_t Fuzzy<string>::search_by_char_presence(const string &line) const
{
    std::unordered_set<char_type> line_set;
    size_t hits = 0;

    for (const auto& ch : line)
        line_set.insert(ch);

    for (const auto& ch : this->m_set)
        if (line_set.count(ch) >= 1)
            hits++;

    return hits;
}
