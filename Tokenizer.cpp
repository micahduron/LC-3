#include "Tokenizer.h"

StringView Tokenizer::nextToken(const CharClass& separatorsClass) {
    StringView token = readUntil(separatorsClass);

    skipUntilNot(separatorsClass);

    return token;
}
size_t Tokenizer::skipUntil(const CharClass& blackList) {
    size_t charsSkipped = 0;

    while (!finished() && !blackList(peek())) {
        advance();
        ++charsSkipped;
    }
    return charsSkipped;
}
size_t Tokenizer::skipUntilNot(const CharClass& whiteList) {
    size_t charsSkipped = 0;

    while (!finished() && whiteList(peek())) {
        advance();
        ++charsSkipped;
    }
    return charsSkipped;
}
StringView Tokenizer::readUntil(const CharClass& blackList) {
    const char* startPtr = m_currPtr;

    size_t numChars = skipUntil(blackList);

    return { startPtr, numChars };
}
StringView Tokenizer::readUntilNot(const CharClass& whiteList) {
    const char* startPtr = m_currPtr;

    size_t numChars = skipUntilNot(whiteList);

    return { startPtr, numChars };
}
