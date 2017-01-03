#include <cstdio>
#include <type_traits>
#include "BinaryFileIO.h"
#include "EndiannessConverter.h"

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
        T convertedDatum = Endianness::encode(datum);

        return std::fwrite(&convertedDatum, sizeof(T), 1, file()) == 1;
    }
};

using BinaryFileWriter = OrderedBinaryFileWriter<Util::Endianness::Converter::NullConverter>;
