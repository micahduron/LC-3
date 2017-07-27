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
    
    StringView nextToken(const CharClass& separatorsClass);

    size_t skip(size_t numChars);
    size_t skipUntil(const CharClass& blackList);
    size_t skipUntilNot(const CharClass& whiteList);

    StringView read(size_t numChars);
    StringView readUntil(const CharClass& blackList);
    StringView readUntilNot(const CharClass& whiteList);

private:
    StringView::iterator m_currIter;
    StringView::iterator m_endIter;
};

}
