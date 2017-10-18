#include <iostream>
#include <tuple>
#include "../language/Tokenizer.h"

using Util::StringView;
using LC3::Language::Token;
using LC3::Language::TokenType;
using LC3::Language::Tokenizer;

std::tuple<const StringView&, TokenType> TokenTuple(const Token& token);

int main() {
    const char* src = ".bloop 43\n \
        instr #42, x56 ; woop\n \
        lol: .FILLZ \"Hello, \\\"world!\\\"\" ; bloop";

    std::tuple<StringView, TokenType> expectedTokens[] = {
        { ".", TokenType::Period },
        { "bloop", TokenType::Word },
        { "43", TokenType::Number },
        { "\n", TokenType::Linebreak },
        { "instr", TokenType::Word },
        { "#", TokenType::Pound },
        { "42", TokenType::Number },
        { ",", TokenType::Comma },
        { "x56", TokenType::Word },
        { "\n", TokenType::Linebreak },
        { "lol", TokenType::Word },
        { ":", TokenType::Colon },
        { ".", TokenType::Period },
        { "FILLZ", TokenType::Word },
        { "Hello, \\\"world!\\\"", TokenType::String }
    };
    size_t expectedTokenSize = sizeof(expectedTokens) / sizeof(expectedTokens[0]);

    Tokenizer tokenizer(src);
    size_t i = 0;

    for (; tokenizer && i < expectedTokenSize; ++i, ++tokenizer) {
        if (TokenTuple(*tokenizer) != expectedTokens[i]) {
            std::cerr << "Mismatch! Position: " << i << ", Expected: "
                      << std::get<TokenType>(expectedTokens[i]) << ", "
                      << "Result: " << (*tokenizer).type << "\n";
            return 1;
        }
    }
    if (i < expectedTokenSize) {
        std::cerr << "Unexpected number of tokens ("
                  << "Expected: " << expectedTokenSize << ", "
                  << "Result: " << i << ")\n";
        return 1;
    }
    if (tokenizer) {
        std::cerr << "More tokens than expected\n";

        return 1;
    }
    if ((*tokenizer).type != TokenType::End) {
        std::cout << "Last token is not of type 'End'\n";

        return 1;
    }
    return 0;
}

std::tuple<const StringView&, TokenType> TokenTuple(const Token& token) {
    return { token.str, token.type };
}
