#pragma once
#include <string>
#include <vector>
#include <unordered_set>

#include <iostream>
#include <locale>
#include <codecvt>

///
/// \brief Fuzzy search mode values
/// \param FZ_SEARCH_BY_CHAR_COUNT ex:
/// pattern: pp
/// words: ppoe, poe
/// result: ppoe
///
/// \param FZ_SEARCH_BY_CHAR_PRESENCE ex:
/// pattern: pp
/// words: ppoe, poe
/// result: ppoe, poe
///
/// \param FZ_SEARCH_BY_CHAR_ORDER ex:
/// pattern: poe
/// words: pose, eop
/// result: pose
///
/// \param FZ_SEARCH_BY_CHAR_ORDER_IGNORE_DUPLICATES ex:
/// pattern: ppooe
/// words: pose, eop
/// result: pose
///
enum {
    FZ_SEARCH_BY_CHAR_COUNT = 0,
    FZ_SEARCH_BY_CHAR_PRESENCE,
    FZ_SEARCH_BY_CHAR_ORDER,
    FZ_SEARCH_BY_CHAR_ORDER_IGNORE_DUPLICATES
};

template <typename string>
class Fuzzy {
    using char_type = typename string::value_type;

public:
    Fuzzy();
    explicit Fuzzy(const string& pattern);
    explicit Fuzzy(const string& pattern, const string& separator);
    ~Fuzzy();

    void process(void);    

    template <typename ch = char_type>
    typename std::enable_if<std::is_same<ch, char>::value, void>::type
    print(void) const {
        std::cout << "Result for pattern: " << this->m_pattern << std::endl;

        for (const auto& line : this->m_result)
            std::cout << line << std::endl;
    }

    template <typename ch = char_type>
    typename std::enable_if<!std::is_same<ch, char>::value, void>::type
    print(void) const {
        std::wstring_convert<std::codecvt_utf8<char_type>, char_type> converter;

        std::cout << "Result for pattern: " << converter.to_bytes(this->m_pattern) << std::endl;

        for (const auto& line : this->m_result)
            std::cout << converter.to_bytes(line) << std::endl;
    }

    template <typename ch = char_type>
    typename std::enable_if<!std::is_same<ch, char>::value, string>::type
    convert(const std::string& str)
    {
        std::wstring_convert<std::codecvt_utf8<char_type>, char_type> converter;
        string out;

        for (const auto& c : str)
            out.push_back(c);

        return out;
    }

    template <typename ch = char_type>
    typename std::enable_if<std::is_same<ch, char>::value, string>::type
    convert(const std::string& str) { return str; }

    void                        set_mode(const int mode) { this->m_mode = mode; }

    void                        set_ignore_case(const bool value) { this->m_ignore_case = value; }

    void                        set_data(const string& data);
    void                        set_data(const std::vector<string>& data);
    void                        set_data(const std::vector<string>* data);

    void                        set_pattern(const string& pattern);
    void                        set_separator(const string& separator);

    std::vector<string>         get_result(void) { return this->m_result; }
    const std::vector<string>&  get_result(void) const { return this->m_result; }

    std::vector<string>         get_data(void) { return this->m_data; }
    const std::vector<string>&  get_data(void) const { return this->m_data; }

    string                      get_pattern(void) { return this->m_pattern; }
    const string&               get_pattern(void) const { return this->m_pattern; }

    string                      get_separator(void) { return this->m_separator; }
    const string&               get_separator(void) const { return this->m_separator; }

private:
    std::vector<string>                 m_data;
    const std::vector<string>*          m_data_ptr;
    std::vector<string>                 m_result;
    std::unordered_set<char_type>       m_set;
    std::unordered_multiset<char_type>  m_multiset;
    string  m_pattern, m_separator;
    bool    m_ignore_case;
    size_t  m_mode;

    const bool prepare(void);

    void separate(const string& text);
    void initialize_sets(void);
    void to_lower(string& str);
    string remove_duplicates(const string& str) const;

    const bool search_by_char_count(const string& line) const;
    const bool search_by_char_presence(const string& line) const;
    const bool search_by_char_order(const string& line) const;
};

template class Fuzzy<std::string>;
template class Fuzzy<std::wstring>;
template class Fuzzy<std::u16string>;
template class Fuzzy<std::u32string>;

