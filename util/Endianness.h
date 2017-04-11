#pragma once

namespace Util {
    enum : int {
        LittleEndian,
        BigEndian,
        SameEndianness,
        OppositeEndianness
    };

    constexpr int GetSystemEndianness() {
        return LittleEndian;
    }
}
