#include <util/StringView.h>
#include "SourceLocation.h"

#pragma once

namespace LC3::Language {

enum class TokenType {
    Comma,
    Period,
    Pound,
    Colon,
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
};

} // namespace LC3::Language
