#pragma once

namespace ByteOrder {
    struct SystemOrder {
        template <typename T>
        T operator () (T value) {
            return value;
        }
    };
}
