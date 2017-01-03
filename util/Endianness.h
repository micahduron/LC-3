#pragma once

namespace Util {
    namespace Endianness {
        enum : int {
            Little,
            Big
        };

        constexpr int GetSystemEndianness() {
            return Little;
        }
    }
}
