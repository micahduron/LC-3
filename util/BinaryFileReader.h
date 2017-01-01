#include <cstdio>
#include <type_traits>
#include "BinaryFileIO.h"
#include "ByteOrder.h"

#pragma once

template <typename Endianness>
class OrderedBinaryFileReader : public BinaryFileIO<FileMode::Reader> {
public:
    using BinaryFileIO::BinaryFileIO;

    template <typename T>
    size_t read(T* buffer, size_t numElems) {
        static_assert(std::is_unsigned<T>::value == true,
                      "Requires unsigned integral type."
        );
        size_t elemsRead = 0;

        for (; elemsRead < numElems; ++elemsRead, ++buffer) {
            if (!get(*buffer)) {
                break;
            }
        }
        return elemsRead;
    }

    template <typename T>
    bool get(T& outputRef) {
        static_assert(std::is_unsigned<T>::value == true,
                      "Requires unsigned integral type."
        );
        T datum;
        size_t bytesRead = std::fread(&datum, sizeof(T), 1, file());

        if (bytesRead < sizeof(T)) {
            seek(-static_cast<long>(bytesRead), SeekMode::FromCurrent);

            return false;
        }
        Endianness converter;
        outputRef = converter(datum);

        return true;
    }
};

using BinaryFileReader = OrderedBinaryFileReader<ByteOrder::SystemOrder>;
