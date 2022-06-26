#include <iostream>
#include "fuzzy.hpp"


int main()
{
    Fuzzy<std::string> fz;

    fz.set_pattern("pokemon");
    fz.set_separator("\n");

    auto out = fz.search("pppppp\n99\n999\nanimal crossing\npokeémon\nsuper mario bros.\npackman\nanimal crossing: new leaf", true);

    return 0;
}
