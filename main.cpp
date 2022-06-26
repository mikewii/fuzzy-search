#include <iostream>
#include "fuzzy.hpp"

using string = std::string;

int main()
{
    Fuzzy<string> fz;

    fz.set_pattern("pokemon");
    fz.set_separator("\n");

    fz.set_data("pppppp\n99\n999\nanimal crossing\npokeémon\nsuper mario bros.\npackman\nanimal crossing: new leaf");

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
