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
    if (this->m_pattern.empty())
        return;

    this->separate(data);

    this->m_result.reserve(this->m_data.size());
}

template<typename string>
void Fuzzy<string>::set_data(const std::vector<string> &data)
{
    if (this->m_pattern.empty())
        return;

    this->m_data = data;

    this->m_result.reserve(this->m_data.size());
}

template<typename string>
void Fuzzy<string>::process(void)
{
    this->m_result.clear();

    for (auto it = this->m_data.begin(); it < this->m_data.end(); it++) {
        std::unordered_multiset<char_type> multiset_line;
        size_t hits = 0;
        string line = *it;

        if (this->m_ignore_case)
            this->to_lower(line);

        for (const auto& ch : line)
            multiset_line.insert(ch);

        for (const auto& ch : this->m_set) {
            const auto& pattern_char_count = this->m_multiset.count(ch);
            const auto& line_char_count = multiset_line.count(ch);

            if (line_char_count >= pattern_char_count)
                hits++;
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
