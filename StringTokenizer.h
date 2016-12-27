#include <string>
#include <algorithm>
#include "StringView.h"
#include "CharClass.h"

#pragma once

class StringTokenizer {
public:
    StringTokenizer(const std::string& sourceStr) :
      m_currPtr{ sourceStr.data() },
      m_endPtr{ sourceStr.data() + sourceStr.size() }
    {}

    bool finished() const {
        return m_currPtr == m_endPtr;
    }
    explicit operator bool() const {
        return finished();
    }

    char peek() const {
        return *m_currPtr;
    }
    char advance() {
        return *++m_currPtr;
    }
    
    StringView nextToken(const CharClass& separatorsClass);

    size_t skip(size_t numChars) {
        size_t ptrDelta = std::min(numChars, static_cast<size_t>(m_endPtr - m_currPtr));

        m_currPtr += ptrDelta;

        return ptrDelta;
    }
    size_t skipUntil(const CharClass& blackList);
    size_t skipUntilNot(const CharClass& whiteList);

    StringView read(size_t numChars) {
        const char* basePtr = m_currPtr;

        size_t viewLen = skip(numChars);

        return { basePtr, viewLen };
    }
    StringView readUntil(const CharClass& blackList);
    StringView readUntilNot(const CharClass& whiteList);

private:
    const char* m_currPtr;
    const char* m_endPtr;
};
