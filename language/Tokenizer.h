#include <iterator>
#include <cstddef>
#include <util/StringView.h>
#include <util/StringTokenizer.h>
#include "SourceLocation.h"
#include "Token.h"

#pragma once

namespace LC3::Language {

class Tokenizer {
private:
    using SrcIter = Util::StringView::iterator;

public:
    using difference_type = std::ptrdiff_t;
    using value_type = Token;
    using pointer = Token*;
    using reference = Token&;
    using iterator_category = std::forward_iterator_tag;

    Tokenizer(const Tokenizer& other) = default;
    Tokenizer(Tokenizer&& other) = default;
    Tokenizer(Util::StringView src) :
      Tokenizer(src.begin(), src.end())
    {}
    Tokenizer(SrcIter startIter, SrcIter endIter) :
      m_tokenizer{ startIter, endIter },
      m_lineStart{ startIter }
    {
        // Assign m_currToken with the first token
        consume(1);
    }

    Tokenizer& operator = (const Tokenizer& other) = default;
    Tokenizer& operator = (Tokenizer&& other) = default;

    bool isDone() const {
        return m_isDone;
    }
    explicit operator bool () const {
        return !isDone();
    }

    void consume(size_t numTokens = 1) {
        if (numTokens == 0) return;

        while (--numTokens) {
            getToken();
        }
        m_currToken = getToken();
    }

    Tokenizer& operator += (size_t numSteps) {
        consume(numSteps);

        return *this;
    }

    Tokenizer& operator ++ () {
        return (*this += 1);
    }
    Tokenizer operator ++ (int) {
        Tokenizer ret = *this;

        *this += 1;

        return ret;
    }

    Tokenizer operator + (size_t numSteps) const {
        Tokenizer ret = *this;

        return (ret += numSteps);
    }

    const Token& peek() const & {
        return m_currToken;
    }
    Token peek() && {
        return m_currToken;
    }
    Token peek(size_t numSkips) const {
        return *(*this + numSkips);
    }

    const Token& operator * () const & {
        return peek();
    }
    Token operator * () && {
        return peek();
    }

private:
    Token getToken();

    SourceLocation getLocation() const {
        Util::StringView src(m_tokenizer.startIter(), m_tokenizer.endIter());
        size_t lineOffset = m_tokenizer.currIter() - m_lineStart;
        size_t absOffset = m_tokenizer.currIter() - m_tokenizer.startIter();

        return { src, m_lineNum, lineOffset, absOffset };
    }

    Util::StringTokenizer m_tokenizer;

    SrcIter m_lineStart;
    size_t m_lineNum = 1;

    bool m_isDone = false;

    Token m_currToken;
};

} // namespace LC3::Language
