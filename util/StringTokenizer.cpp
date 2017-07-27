#include "StringTokenizer.h"

namespace Util {

StringView StringTokenizer::nextToken(const CharClass& separatorsClass) {
    skipUntilNot(separatorsClass);

    StringView token = readUntil(separatorsClass);

    return token;
}
size_t StringTokenizer::skip(size_t numChars) {
    size_t ptrDelta = std::min(numChars, static_cast<size_t>(remaining()));

    m_currIter += ptrDelta;

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
    auto startIter = m_currIter;

    size_t viewLen = skip(numChars);

    return { startIter, startIter + viewLen };
}
StringView StringTokenizer::readUntil(const CharClass& blackList) {
    auto startIter = m_currIter;

    size_t numChars = skipUntil(blackList);

    return { startIter, startIter + numChars };
}
StringView StringTokenizer::readUntilNot(const CharClass& whiteList) {
    auto startIter = m_currIter;

    size_t numChars = skipUntilNot(whiteList);

    return { startIter, startIter + numChars };
}

}
