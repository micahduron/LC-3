#pragma once

namespace Util {
    enum : int {
        LittleEndian,
        BigEndian,
        SameEndianness,
        OppositeEndianness
    };

    namespace Internals {
        constexpr int GetSystemEndianness() {
            return LittleEndian;
        }
    }
    constexpr int SystemEndianness = Internals::GetSystemEndianness();
}
