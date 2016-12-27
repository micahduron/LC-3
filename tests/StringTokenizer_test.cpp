#include <iostream>
#include <cstring>
#include "../CharClass.h"
#include "../StringTokenizer.h"

int main() {
    CharClass isSpace{ [](char c) { return isspace(c); } };
    CharClass isComma{ [](char c) { return c == ','; } };

    auto separators = isSpace | isComma;

    std::string sourceStr = "One, two, three, four";

    StringTokenizer tokenizer(sourceStr);

    while (tokenizer) {
        auto numberStr = tokenizer.nextToken(separators);
        std::cout << numberStr << ": " << numberStr.size() << '\n';
    }
    return 0;
}
