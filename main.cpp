#include <iostream>
#include "fuzzy.hpp"

using string = std::string;

int main()
{
    Fuzzy<string> fz;

    std::vector<string> data{"pppppp", "99", "999", "animal crossing", "Pokeémonß", "super mario bros.", "packman", "animal crossing: new leaf"};

    fz.set_pattern("aacnnll");
    fz.set_separator("\n");
    fz.set_ignore_case(true);

    fz.set_mode(FZ_SEARCH_BY_CHAR_ORDER_IGNORE_DUPLICATES);

    //fz.set_data("pppppp\n99\n999\nanimal crossing\nPokeémonß\nsuper mario bros.\npackman\nanimal crossing: new leaf");
    fz.set_data(data);

    string new_pattern = "";

    for (const auto& ch : fz.get_pattern()) {
        new_pattern.push_back(ch);

        fz.set_pattern(new_pattern);

        fz.process();
        fz.print();
    }

    auto out = fz.get_result();

    return 0;
}
