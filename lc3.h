#pragma once

#include <iostream>
#include <cstdint>
#include <climits>
#include <optional>

namespace LC3 {
    using Word = std::uint16_t;

    constexpr size_t WordBytes = sizeof(Word);
    constexpr size_t WordBits = WordBytes * CHAR_BIT;

    class Value {
    public:
        Value(Word word) :
          m_word{ word }
        {}

        Word get() const {
            return m_word;
        }

        operator Word () const {
            return get();
        }

        Value abs() const;
        size_t bitWidth() const;
        std::optional<Value> restrictWidth(size_t bitWidth, bool isSigned = false) const;

    private:
        Word m_word = 0;
    };

    std::ostream& operator << (std::ostream& outStream, const Value& val);
}
