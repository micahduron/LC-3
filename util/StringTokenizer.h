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
    StringTokenizer(StringView source) :
      m_currIter{ source.begin() },
      m_endIter{ source.end() }
    {}

    StringTokenizer& operator = (const StringTokenizer& other) = default;

    std::ptrdiff_t remaining() const {
        return m_endIter - m_currIter;
    }
    bool finished() const {
        return remaining() <= 0;
    }
    explicit operator bool() const {
        return !finished();
    }

    char peek() const {
        return *m_currIter;
    }
    char advance() {
        return *++m_currIter;
    }
    
    Util::StringView nextToken(const Util::CharClass& separatorsClass);

    size_t skip(size_t numChars);
    size_t skipUntil(const Util::CharClass& blackList);
    size_t skipUntilNot(const Util::CharClass& whiteList);

    Util::StringView read(size_t numChars);
    Util::StringView readUntil(const Util::CharClass& blackList);
    Util::StringView readUntilNot(const Util::CharClass& whiteList);

private:
    StringView::iterator m_currIter;
    StringView::iterator m_endIter;
};

}
