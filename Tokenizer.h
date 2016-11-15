#include <string>
#include "StringView.h"
#include "CharClass.h"

#pragma once

enum TokenizeBehavior : uint32_t {
    IgnoreWhitespace = 1 << 0,
    IncludeSeparator = 1 << 1
};

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
    
    StringView read(size_t numChars) {
        return { m_currPtr += numChars, numChars };
    }
    StringView readUntil(const CharClass& blackList);
    StringView readUntilNot(const CharClass& whiteList);

private:
    const char* m_currPtr;
    const char* m_endPtr;
};
