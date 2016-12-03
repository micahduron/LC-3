#include <iostream>
#include <cstring>
#include "../CharClass.h"
#include "../Tokenizer.h"

int main() {
    CharClass isSpace{ [](char c) { return isspace(c); } };
    CharClass isComma{ [](char c) { return c == ','; } };

    auto separators = isSpace | isComma;

    std::string sourceStr = "One, two, three, four";

    Tokenizer tokenizer(sourceStr);

    while (!tokenizer.finished()) {
        auto numberStr = tokenizer.nextToken(separators);
        std::cout << numberStr << ": " << numberStr.size() << '\n';
    }
    return 0;
}
