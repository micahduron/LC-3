#include <string>
#include <cstddef>
#include <algorithm>
#include "StringView.h"
#include "CharClass.h"

#pragma once

namespace Util {

class StringTokenizer {
public:
    StringTokenizer(const StringTokenizer& other) = default;
    StringTokenizer(const char* sourceStr, size_t strLen) :
      m_currPtr{ sourceStr },
      m_endPtr{ sourceStr + strLen }
    {}
    StringTokenizer(const std::string& sourceStr) :
      m_currPtr{ sourceStr.data() },
      m_endPtr{ sourceStr.data() + sourceStr.size() }
    {}
    StringTokenizer(const Util::StringView& sourceStr) :
      m_currPtr{ sourceStr.data() },
      m_endPtr{ sourceStr.data() + sourceStr.size() }
    {}

    StringTokenizer& operator = (const StringTokenizer& other) = default;

    std::ptrdiff_t remaining() const {
        return m_endPtr - m_currPtr;
    }
    bool finished() const {
        return remaining() <= 0;
    }
    explicit operator bool() const {
        return !finished();
    }

    char peek() const {
        return *m_currPtr;
    }
    char advance() {
        return *++m_currPtr;
    }
    
    Util::StringView nextToken(const Util::CharClass& separatorsClass);

    size_t skip(size_t numChars);
    size_t skipUntil(const Util::CharClass& blackList);
    size_t skipUntilNot(const Util::CharClass& whiteList);

    Util::StringView read(size_t numChars);
    Util::StringView readUntil(const Util::CharClass& blackList);
    Util::StringView readUntilNot(const Util::CharClass& whiteList);

private:
    const char* m_currPtr;
    const char* m_endPtr;
};

}
