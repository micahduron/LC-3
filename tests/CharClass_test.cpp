#include <iostream>
#include <cstring>
#include "../util/CharClass.h"

using Util::CharClass;

int main() {
    CharClass isAlpha{ [](char c)->bool { return isalpha(c); } };
    auto isLetterJ = CharClass::matchSingle('J');
    CharClass isNum{ [](char c)->bool { return c >= '0' && c <= '9'; } };

    auto isAlnum = isAlpha | isNum;

    auto isNotJ = ~isLetterJ & isAlpha;

    if (isAlnum('a') && isAlnum('5') && isNotJ('a') && !isNotJ('J')) {
        std::cout << "Yep.\n";
    } else {
        std::cout << "Nope.\n";
    }
    return 0;
}
