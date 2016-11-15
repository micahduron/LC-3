#include <string>
#include <algorithm>
#include "StringView.h"
#include "CharClass.h"

#pragma once

class Tokenizer {
public:
    Tokenizer(const std::string& sourceStr) :
      m_currPtr{ sourceStr.data() },
      m_endPtr{ sourceStr.data() + sourceStr.size() }
    {}

    bool finished() const {
        return m_currPtr == m_endPtr;
    }
    char peek() const {
        return *m_currPtr;
    }
    char advance() {
        return *++m_currPtr;
    }
    
    size_t skip(size_t numChars) {
        size_t ptrDelta = std::min(numChars, static_cast<size_t>(m_endPtr - m_currPtr));

        m_currPtr += ptrDelta;

        return ptrDelta;
    }
    size_t skipUntil(const CharClass& blackList);
    size_t skipUntilNot(const CharClass& whiteList);

    StringView read(size_t numChars) {
        return { m_currPtr += numChars, numChars };
    }
    StringView readUntil(const CharClass& blackList);
    StringView readUntilNot(const CharClass& whiteList);

private:
    const char* m_currPtr;
    const char* m_endPtr;
};
