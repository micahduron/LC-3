#pragma once

namespace Util::Endianness {
    enum : int {
        Little,
        Big
    };

    constexpr int GetSystemEndianness() {
        return Little;
    }
}
