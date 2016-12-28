#include <cstdint>

#pragma once

namespace LC3 {
    using Word = std::uint16_t;

    constexpr size_t WordBytes = 2;
    constexpr size_t WordBits = WordBytes * 8;
};
