#include <cstddef>
#include <limits>
#include <bitset>
#include "StringView.h"

#pragma once

namespace Util {

class CharClass {
private:
    static constexpr size_t SetSize = std::numeric_limits<uint8_t>::max() + 1;
    using BitSet = std::bitset<SetSize>;

public:
    CharClass(const CharClass& other) {
        // std::bitset doesn't have a normal copy constructor, weirdly.
        for (size_t i = 0; i < SetSize; ++i) {
            if (other.m_bitSet.test(i)) {
                m_bitSet.set(i, true);
            }
        }
    }
    CharClass(StringView acceptedChars) {
        for (char c : acceptedChars) {
            set(c, true);
        }
    }
    template <typename MapFn,
              typename = decltype(std::declval<MapFn>()('a'))>
    CharClass(MapFn mapFn) {
        for (size_t i = 0; i < SetSize; ++i) {
            char charVal = static_cast<char>(i);

            if (mapFn(charVal)) {
                m_bitSet.set(i, true);
            }
        }
    }

    CharClass& operator = (const CharClass& other) = delete;

    bool operator () (char c) const {
        return get(c);
    }

    static CharClass combine(const CharClass& classOne, const CharClass& classTwo);
    static CharClass intersect(const CharClass& classOne, const CharClass& classTwo);
    static CharClass complement(const CharClass& charClass);

private:
    bool get(char c) const {
        return m_bitSet.test(static_cast<uint8_t>(c));
    }
    void set(char c, bool value) {
        m_bitSet.set(static_cast<uint8_t>(c), value);
    }

    BitSet m_bitSet;
};

CharClass operator ~ (const CharClass& charClass);
CharClass operator & (const CharClass& classOne, const CharClass& classTwo);
CharClass operator | (const CharClass& classOne, const CharClass& classTwo);

}
