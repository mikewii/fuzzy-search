#include "fuzzy.hpp"
#include <algorithm>
#include <set>

template<typename string>
Fuzzy<string>::Fuzzy()
    : m_ignore_case(false), m_ignore_duplicates(false)
    , m_mode(FZ_SEARCH_BY_CHAR_COUNT)
{
}

template<typename string>
Fuzzy<string>::Fuzzy(const string &pattern)
    : m_ignore_case(false), m_ignore_duplicates(false)
    , m_mode(FZ_SEARCH_BY_CHAR_COUNT)
{
    this->set_pattern(pattern);
}

template<typename string>
Fuzzy<string>::Fuzzy(const string &pattern, const string &separator)
    : m_separator(separator), m_ignore_case(false)
    , m_ignore_duplicates(false), m_mode(FZ_SEARCH_BY_CHAR_COUNT)
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
const bool Fuzzy<string>::prepare(void)
{
    this->m_result.clear();

    if (this->m_pattern.empty()) {
        return false;
    }

    if (this->m_ignore_case)
        this->set_pattern(this->m_pattern);

    if (this->m_ignore_duplicates)
        this->set_pattern(this->remove_duplicates(this->m_pattern));

    return true;
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
    if (!this->prepare()) {
        this->m_result.push_back(this->convert("Fuzzy:: search pattern is empty!"));
        return;
    }

    for (auto it = this->m_data.begin(); it < this->m_data.end(); it++) {
        string line = *it;
        bool push = false;

        if (this->m_ignore_case)
            this->to_lower(line);

        switch (this->m_mode) {
        default:
        case FZ_SEARCH_BY_CHAR_COUNT:
            push = this->search_by_char_count(line);
            break;
        case FZ_SEARCH_BY_CHAR_PRESENCE:
            push = this->search_by_char_presence(line);
            break;
        case FZ_SEARCH_BY_CHAR_ORDER:
            push = this->search_by_char_order(line);
            break;
        }

        if (push)
            this->m_result.push_back(*it);
    }
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
string Fuzzy<string>::remove_duplicates(const string &str) const
{
    std::unordered_set<char_type> set;
    string out = str;

    auto filter = [&] (const char_type c)
    {
        return !(set.insert(c).second);
    };

    out.erase(std::remove_if(out.begin(), out.end(), filter), out.end());

    return out;
}

template<typename string>
const bool Fuzzy<string>::search_by_char_count(const string &line) const
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

    return hits == this->m_set.size();
}

template<typename string>
const bool Fuzzy<string>::search_by_char_presence(const string &line) const
{
    std::unordered_set<char_type> line_set;
    size_t hits = 0;

    for (const auto& ch : line)
        line_set.insert(ch);

    for (const auto& ch : this->m_set)
        if (line_set.count(ch) >= 1)
            hits++;

    return hits == this->m_set.size();
}

template<typename string>
const bool Fuzzy<string>::search_by_char_order(const string &line) const
{
    size_t hits = 0, next_pos = 0;

    for (size_t i = 0; i < this->m_pattern.length(); i++) {
        const auto& ch = this->m_pattern.at(i);

        for (size_t pos = next_pos; pos < line.length(); pos++) {
            const auto& line_ch = line.at(pos);

            next_pos = pos;

            if (line_ch == ch) {
                hits++;
                break;
            }
        }
    }

    return hits == this->m_pattern.length();
}
