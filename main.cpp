#include <iostream>
#include "fuzzy.hpp"

using string = std::u16string;

int main()
{
    Fuzzy<string> fz;

    fz.set_pattern(u"pokemon");
    fz.set_separator(u"\n");
    fz.set_ignore_case(true);

    fz.set_data(u"pppppp\n99\n999\nanimal crossing\nPokeémonß\nsuper mario bros.\npackman\nanimal crossing: new leaf");

    string new_pattern = u"";

    for (const auto& ch : fz.get_pattern()) {
        new_pattern.push_back(ch);

        fz.set_pattern(new_pattern);

        fz.process();
        fz.print();
    }

    auto out = fz.get_result();

    return 0;
}
