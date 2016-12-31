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
        size_t bytesWritten = 0;

        while (numElems--) {
            size_t elemBytesWritten = put(*data);
            bytesWritten += elemBytesWritten;

            if (elemBytesWritten != sizeof(T)) {
                break;
            }
            ++data;
        }
        return bytesWritten;
    }

    template <typename T>
    size_t put(T datum) {
        static_assert(std::is_unsigned<T>::value == true,
                      "Requires unsigned integral type."
        );
        Endianness converter;
        auto convertedDatum = converter(datum);

        return std::fwrite(&convertedDatum, sizeof(T), 1, file());
    }
};

template <>
class OrderedBinaryFileWriter<ByteOrder::SystemEndian> : public BinaryFileIO<FileMode::Writer> {
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

using BinaryFileWriter = OrderedBinaryFileWriter<ByteOrder::SystemEndian>;
