#pragma once

namespace ByteOrder {
    class SystemEndian {
    public:
        template <typename T>
        T operator () (T value) {
            return value;
        }
    };
}
