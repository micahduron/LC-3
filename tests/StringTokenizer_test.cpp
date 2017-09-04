#include <iostream>
#include <array>
#include <vector>
#include <cstring>
#include <util/CharClass.h>
#include <util/StringTokenizer.h>
#include <util/StringView.h>

using Util::CharClass;
using Util::StringTokenizer;
using Util::StringView;

std::vector<StringView> TokenizeString(const char* str);

int main() {
    const char* sourceStr = "One, two, three, four";
    std::array expectedTokens{ "One", "two", "three", "four" };
    std::vector<StringView> calculatedTokens = TokenizeString(sourceStr);

    if (calculatedTokens.size() != expectedTokens.size()) {
        std::cerr << "Unexpected number of tokens (" <<
            "Calculated: " << calculatedTokens.size() << ", " <<
            "Expected: " << expectedTokens.size() << ")\n";
        return 1;
    }
    for (size_t i = 0; i < expectedTokens.size(); ++i) {
        if (calculatedTokens[i] != expectedTokens[i]) {
            std::cerr << "Unexpected token (" <<
                "Calculated: " << calculatedTokens[i] << ", " <<
                "Expected: " << expectedTokens[i] << ")\n";
            return 1;
        }
    }
    return 0;
}

std::vector<StringView> TokenizeString(const char* str) {
    std::vector<StringView> result;
    StringTokenizer tokenizer(str);

    CharClass isSpace{ [](char c) { return isspace(c); } };
    CharClass isComma{ [](char c) { return c == ','; } };

    auto separators = isSpace | isComma;

    while (tokenizer) {
        StringView numberToken = tokenizer.nextToken(separators);
        result.push_back(numberToken);
    }
    return result;
}
