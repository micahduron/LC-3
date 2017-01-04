#include <cstdint>
#include <climits>

#pragma once

namespace LC3 {
    using Word = std::uint16_t;

    constexpr size_t WordBytes = sizeof(Word);
    constexpr size_t WordBits = WordBytes * CHAR_BIT;
}
