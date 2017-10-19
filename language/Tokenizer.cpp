#include <stdexcept>
#include <cctype>
#include <cassert>
#include <util/CharClass.h>
#include <util/StringView.h>
#include <util/StringTokenizer.h>
#include "Tokenizer.h"

namespace LC3::Language {

using Util::CharClass;
using Util::StringView;
using Util::StringTokenizer;

static const CharClass isPunct(".,#:");
static const CharClass isQuote("'\"");
static const CharClass isComment(";");
static const CharClass isNewline("\n");
static const CharClass isSpace = ~isNewline & CharClass([](char c) -> bool {
    auto cu = static_cast<unsigned char>(c);

    return std::isspace(cu) != 0;
});
static const CharClass isAlpha([](char c) -> bool {
    auto cu = static_cast<unsigned char>(c);

    return std::isalpha(cu) != 0;
});
static const CharClass isDecDigit("0123456789");
static const CharClass isHexDigit("0123456789abcdefABCDEF");
static const CharClass isAlnum = isAlpha | isDecDigit;
static const CharClass isWord = isAlnum | CharClass("_");
static const CharClass isEnd("\0"_sv);

Token Tokenizer::getToken() {
    StringTokenizer& tokenizer = m_tokenizer;
    char nextChar = tokenizer.peekChar();

    if (isSpace(nextChar)) {
        tokenizer.skipUntilNot(isSpace);

        return getToken();
    }
    if (isComment(nextChar)) {
        tokenizer.skipUntil(isNewline);

        return getToken();
    }
    StringView tokenStr;
    auto tokenType = TokenType::Unknown;
    auto tokenLocation = getLocation();

    if (isNewline(nextChar)) {
        tokenStr = tokenizer.read(1);
        tokenType = TokenType::Linebreak;

        ++m_lineNum;
        m_lineStart = tokenizer.currIter();
    } else if (isPunct(nextChar)) {
        tokenStr = tokenizer.read(1);

        switch (nextChar) {
            case '.':
                tokenType = TokenType::Period;
                break;
            case ',':
                tokenType = TokenType::Comma;
                break;
            case ':':
                tokenType = TokenType::Colon;
                break;
            case '#':
                tokenType = TokenType::Pound;
                break;
            default:
                throw std::domain_error("Unimplemented token-punctuation case.");
        }
    } else if (isDecDigit(nextChar)) {
        // Consuming all alphanumeric characters in this situation will help
        // with things further down the assembling pipeline. For example,
        // consider the text '0abcd'. If only numeric characters were
        // consumed, this text would be split into two tokens: '0' and
        // 'abcd'. This would create problems when generating error
        // messages and such.
        tokenStr = tokenizer.readUntilNot(isAlnum);
        tokenType = TokenType::Number;
    } else if (isWord(nextChar)) {
        tokenStr = tokenizer.readUntilNot(isWord);
        tokenType = TokenType::Word;
    } else if (isQuote(nextChar)) {
        char quoteChar = nextChar;

        // Skip over the opening quote
        tokenizer.skip(1);

        auto startPos = tokenizer.currIter();
        size_t tokenLength = 0;

        for (; !tokenizer.finished(); tokenizer.skip(1), ++tokenLength) {
            if (tokenizer.peekChar() == quoteChar) break;

            if (tokenizer.peekChar() == '\\') {
                tokenizer.skip(1);

                if (tokenizer.peekChar() == quoteChar) {
                    ++tokenLength;
                }
            }
        }
        // Skip over the closing quote
        tokenizer.skip(1);

        tokenStr = StringView(startPos, startPos + tokenLength);
        tokenType = TokenType::String;
    } else if (isEnd(nextChar)) {
        tokenType = TokenType::End;
        m_isDone = true;
    } else {
        tokenStr = tokenizer.read(1);
        tokenType = TokenType::Unknown;
    }
    return { tokenType, tokenStr, tokenLocation };
}

} // namespace LC3::Language
