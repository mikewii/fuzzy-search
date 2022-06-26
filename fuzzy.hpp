#pragma once
#include <string>
#include <vector>
#include <unordered_set>

#include <iostream>
#include <locale>
#include <codecvt>

template <typename string>
class Fuzzy {
    using char_type = typename string::value_type;

public:
    Fuzzy(){};
    Fuzzy(const string& pattern);

    void search(const string& text);
    void search(const std::vector<string>& vector);

    void process(const std::vector<string>& vector);

    void set_pattern(const string& pattern);
    void set_separator(const string& separator);

    template <typename ch = char_type>
    typename std::enable_if<std::is_same<ch, char>::value, void>::type
    print(void) const {
        for (const auto& line : this->m_result) {
            std::cout << line << std::endl;
        }
    }

    template <typename ch = char_type>
    typename std::enable_if<!std::is_same<ch, char>::value, void>::type
    print(void) const {
        std::wstring_convert<std::codecvt_utf8<char_type>, char_type> converter;
        for (const auto& line : this->m_result) {
            std::cout << converter.to_bytes(line) << std::endl;
        }
    }

    std::vector<string> get_result(void) const { return this->m_result; }


private:
    std::vector<string>                 m_result;
    std::unordered_set<char_type>       m_set;
    std::unordered_multiset<char_type>  m_multiset;
    string  m_pattern, m_separator, m_pattern_size;

    const std::vector<string> separate(const string& text);
    void make_sets(void);
};

template class Fuzzy<std::string>;
template class Fuzzy<std::u16string>;
template class Fuzzy<std::u32string>;
