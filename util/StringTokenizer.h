#include <string>
#include <cstddef>
#include <cassert>
#include <algorithm>
#include "StringView.h"
#include "CharClass.h"

#pragma once

namespace Util {

class StringTokenizer {
public:
    using StrIter = StringView::iterator;

    StringTokenizer(const StringTokenizer& other) = default;
    StringTokenizer(StringView source) :
      StringTokenizer{ source.begin(), source.end() }
    {}
    StringTokenizer(StrIter startIter, StrIter endIter) :
      m_startIter{ startIter },
      m_currIter{ startIter },
      m_endIter{ endIter }
    {}

    StringTokenizer& operator = (const StringTokenizer& other) = default;

    size_t position() const {
        return static_cast<size_t>(m_currIter - m_startIter);
    }
    size_t remaining() const {
        return static_cast<size_t>(m_endIter - m_currIter);
    }
    bool finished() const {
        return remaining() == 0;
    }
    explicit operator bool() const {
        return !finished();
    }

    char peekChar(size_t offset = 0) const {
        return *StringTokenizer::fetch(m_currIter, m_endIter, offset);
    }

    StringView peekStr(size_t length, size_t offset = 0) const {
        StrIter start = StringTokenizer::fetch(m_currIter, m_endIter, offset);
        StrIter end = StringTokenizer::fetch(start, m_endIter, length);

        return { start, end };
    }
    char advance() {
        return *++m_currIter;
    }
    
    StringView nextToken(const CharClass& separatorsClass) {
        skipUntilNot(separatorsClass);

        StringView token = readUntil(separatorsClass);

        skipUntilNot(separatorsClass);

        return token;
    }

    size_t skip(size_t numChars) {
        StrIter oldIter = m_currIter;
        m_currIter = StringTokenizer::fetch(m_currIter, m_endIter, numChars);

        return m_currIter - oldIter;
    }
    size_t skipUntil(const CharClass& blacklist) {
        StrIter oldIter = m_currIter;
        m_currIter = StringTokenizer::fetchUntil(m_currIter, m_endIter, blacklist);

        return m_currIter - oldIter;
    }
    size_t skipUntilNot(const CharClass& whitelist) {
        StrIter oldIter = m_currIter;
        m_currIter = StringTokenizer::fetchUntilNot(m_currIter, m_endIter, whitelist);

        return m_currIter - oldIter;
    }

    StringView read(size_t numChars) {
        StrIter oldIter = m_currIter;
        m_currIter = StringTokenizer::fetch(m_currIter, m_endIter, numChars);

        return { oldIter, m_currIter };
    }
    StringView readUntil(const CharClass& blacklist) {
        StrIter oldIter = m_currIter;
        m_currIter = StringTokenizer::fetchUntil(m_currIter, m_endIter, blacklist);

        return { oldIter, m_currIter };
    }
    StringView readUntilNot(const CharClass& whitelist) {
        StrIter oldIter = m_currIter;
        m_currIter = StringTokenizer::fetchUntilNot(m_currIter, m_endIter, whitelist);

        return { oldIter, m_currIter };
    }

private:
    static StrIter fetch(const StrIter& start, const StrIter& end, size_t numChars) {
        assert(end >= start);

        size_t rangeLen = end - start;

        return (numChars <= rangeLen) ? start + numChars : end;
    }
    static StrIter fetchUntil(const StrIter& start, const StrIter& end, const CharClass& blacklist) {
        assert(end >= start);

        StrIter curr = start;

        while (curr != end && !blacklist(*curr)) {
            ++curr;
        }
        return curr;
    }
    static StrIter fetchUntilNot(const StrIter& start, const StrIter& end, const CharClass& whitelist) {
        assert(end >= start);

        StrIter curr = start;

        while (curr != end && whitelist(*curr)) {
            ++curr;
        }
        return curr;
    }

private:
    StrIter m_startIter;
    StrIter m_currIter;
    StrIter m_endIter;
};

}
