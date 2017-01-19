#include "StringTokenizer.h"

StringView StringTokenizer::nextToken(const CharClass& separatorsClass) {
    skipUntilNot(separatorsClass);

    StringView token = readUntil(separatorsClass);

    return token;
}
size_t StringTokenizer::skip(size_t numChars) {
    size_t ptrDelta = std::min(numChars, static_cast<size_t>(m_endPtr - m_currPtr));

    m_currPtr += ptrDelta;

    return ptrDelta;
}
size_t StringTokenizer::skipUntil(const CharClass& blackList) {
    size_t charsSkipped = 0;

    while (!finished() && !blackList(peek())) {
        advance();
        ++charsSkipped;
    }
    return charsSkipped;
}
size_t StringTokenizer::skipUntilNot(const CharClass& whiteList) {
    size_t charsSkipped = 0;

    while (!finished() && whiteList(peek())) {
        advance();
        ++charsSkipped;
    }
    return charsSkipped;
}
StringView StringTokenizer::read(size_t numChars) {
    const char* basePtr = m_currPtr;

    size_t viewLen = skip(numChars);

    return { basePtr, viewLen };
}
StringView StringTokenizer::readUntil(const CharClass& blackList) {
    const char* startPtr = m_currPtr;

    size_t numChars = skipUntil(blackList);

    return { startPtr, numChars };
}
StringView StringTokenizer::readUntilNot(const CharClass& whiteList) {
    const char* startPtr = m_currPtr;

    size_t numChars = skipUntilNot(whiteList);

    return { startPtr, numChars };
}
