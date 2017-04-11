#pragma once

namespace Util {
    enum : int {
        LittleEndian,
        BigEndian,
    };

    constexpr int GetSystemEndianness() {
        return LittleEndian;
    }
}
