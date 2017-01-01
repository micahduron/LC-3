#include <cstdio>
#include <cstdint>
#include <type_traits>
#include "BinaryFileIO.h"
#include "ByteOrder.h"

#pragma once

template <typename Endianness>
class OrderedBinaryFileWriter : public BinaryFileIO<FileMode::Writer> {
public:
    using BinaryFileIO::BinaryFileIO;

    template <typename T>
    size_t write(const T* data, size_t numElems) {
        static_assert(std::is_unsigned<T>::value == true,
                      "Requires unsigned integral type."
        );
        size_t elemsWritten = 0;

        for (; elemsWritten < numElems; ++elemsWritten, ++data) {
            if (!put(*data)) {
                break;
            }
        }
        return elemsWritten;
    }

    template <typename T>
    bool put(T datum) {
        static_assert(std::is_unsigned<T>::value == true,
                      "Requires unsigned integral type."
        );
        Endianness converter;
        auto convertedDatum = converter(datum);

        return std::fwrite(&convertedDatum, sizeof(T), 1, file()) == sizeof(T);
    }
};

template <>
class OrderedBinaryFileWriter<ByteOrder::SystemOrder> : public BinaryFileIO<FileMode::Writer> {
public:
    using BinaryFileIO::BinaryFileIO;

    template <typename T>
    size_t write(const T* data, size_t numElems) {
        static_assert(std::is_unsigned<T>::value == true,
                      "Requires unsigned integral type."
        );
        return std::fwrite(data, sizeof(T), numElems, file());
    }

    template <typename T>
    size_t put(T datum) {
        static_assert(std::is_unsigned<T>::value == true,
                      "Requires unsigned integral type."
        );
        return std::fwrite(&datum, sizeof(T), 1, file());
    }
};

using BinaryFileWriter = OrderedBinaryFileWriter<ByteOrder::SystemOrder>;
