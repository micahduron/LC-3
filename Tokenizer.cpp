#include "Tokenizer.h"

StringView Tokenizer::readUntil(const CharClass& blackList) {
    const char* startPtr = m_currPtr;
    size_t numChars = 0;

    while (!finished() && !blackList(peek())) {
        advance();

        ++numChars;
    }
    while (!finished() && blackList(peek())) {
        advance();
    }
    return { startPtr, numChars };
}
StringView Tokenizer::readUntilNot(const CharClass& whiteList) {
    const char* startPtr = m_currPtr;
    size_t numChars = 0;

    while (!finished() && whiteList(peek())) {
        advance();

        ++numChars;
    }
    while(!finished() && !whiteList(peek())) {
        advance();
    }
    return { startPtr, numChars };
}
