#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <util/StringView.h>
#include "SourceLocation.h"

#pragma once

namespace LC3::Language {

enum class TokenType {
    Comma,
    Period,
    Pound,
    Colon,
    Minus,
    Word,
    Number,
    String,
    Linebreak,
    End,
    Unknown
};

struct Token {
    TokenType type = TokenType::Unknown;
    Util::StringView str;
    SourceLocation location;

    std::ostream& errorPrefix(std::ostream& errStream) const {
        errStream << location << "\n" << location.getLine() << "\n";

        return errStream;
    }
};

inline std::ostream& operator << (std::ostream& outStream, TokenType tokenType) {
    switch (tokenType) {
    #define CASE(type) \
        case TokenType::type: \
            outStream << #type; \
            break

        CASE(Comma);
        CASE(Period);
        CASE(Pound);
        CASE(Colon);
        CASE(Minus);
        CASE(Word);
        CASE(Number);
        CASE(String);
        CASE(Linebreak);
        CASE(End);
        CASE(Unknown);

    #undef CASE

        default:
            throw std::domain_error("Unimplemented TokenType case.");
    }
    return outStream;
}

} // namespace LC3::Language
