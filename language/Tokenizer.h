#include <iterator>
#include <cstddef>
#include <util/StringView.h>
#include <util/StringTokenizer.h>
#include "SourceLocation.h"
#include "Token.h"

#pragma once

namespace LC3::Language {

namespace Internals {

struct TokenizerState {
private:
    using SrcIter = Util::StringView::iterator;

public:
    Util::StringTokenizer tokenizer;
    SrcIter lineStart;
    size_t lineNum = 0;

    bool isDone = false;

    TokenizerState(SrcIter startIter, SrcIter endIter) :
      tokenizer{ startIter, endIter },
      lineStart{ startIter }
    {}

    SourceLocation getLocation() const {
        Util::StringView src(tokenizer.startIter(), tokenizer.endIter());
        size_t lineOffset = tokenizer.currIter() - lineStart;
        size_t absOffset = tokenizer.currIter() - tokenizer.startIter();

        return { src, lineNum, lineOffset, absOffset };
    }
};

} // namespace LC3::Language::Internals

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
      m_state{ startIter, endIter }
    {
        // Assign m_currToken with the first token
        consume(1);
    }

    Tokenizer& operator = (const Tokenizer& other) = default;
    Tokenizer& operator = (Tokenizer&& other) = default;

    bool isDone() const {
        return m_state.isDone;
    }
    explicit operator bool () const {
        return !isDone();
    }

    void consume(size_t numTokens = 1) {
        if (numTokens > 0) {
            m_currToken = Tokenizer::getToken(m_state, numTokens);
        }
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

    const Token& peek() const {
        return m_currToken;
    }
    Token peek(size_t numSkips) const {
        return *(*this + numSkips);
    }

    const Token& operator * () const {
        return m_currToken;
    }

private:
    static Token getToken(Internals::TokenizerState& state, size_t numSteps);

    Internals::TokenizerState m_state;
    Token m_currToken;
};

} // namespace LC3::Language
