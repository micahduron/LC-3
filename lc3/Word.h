#pragma once

#include <cstdint>
#include <iomanip>
#include <iostream>

namespace LC3 {

using WordValue = std::uint16_t;

class Word {
public:
    using value_type = WordValue;

    static constexpr auto maxValue = static_cast<value_type>(-1);
    static constexpr size_t numBytes = sizeof(value_type);
    static constexpr size_t numBits = numBytes * 8;

    constexpr Word() = default;

    constexpr Word(value_type value) :
      m_value{ value }
    {}

    constexpr Word& operator = (value_type wordVal) {
        m_value = wordVal;

        return *this;
    }

    explicit constexpr operator value_type () const {
        return m_value;
    }

    constexpr value_type value() const {
        return m_value;
    }

    constexpr Word abs() const {
        constexpr WordValue signMask = 1 << (Word::numBits - 1);

        return (value() & signMask) ? negate() : *this;
    }

    constexpr Word negate() const {
        return Word(~m_value + 1);
    }

    constexpr Word operator - () const {
        return negate();
    }

    constexpr Word operator + (Word otherWord) const {
        return Word(m_value + otherWord.value());
    }

    constexpr Word operator << (int shiftVal) const {
        return Word(m_value << shiftVal);
    }

    constexpr Word operator ~ () const {
        return Word(~m_value);
    }

    constexpr Word operator & (Word otherWord) const {
        return Word(m_value & otherWord.value());
    }

    constexpr Word operator | (Word otherWord) const {
        return Word(m_value | otherWord.value());
    }

private:
    value_type m_value = 0x0000;
};

inline std::ostream& operator << (std::ostream& outStream, Word word) {
    return (outStream << "0x" << std::hex << std::setfill('0') << std::setw(4) << word.value() << std::dec);
}

} // namespace LC3
