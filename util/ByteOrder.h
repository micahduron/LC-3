#pragma once

namespace ByteOrder {
    class SystemOrder {
    public:
        template <typename T>
        T operator () (T value) {
            return value;
        }
    };
}
