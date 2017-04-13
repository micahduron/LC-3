#include <cstdio>
#include <type_traits>
#include "BinaryIO.h"
#include "FileMode.h"
#include "EndiannessConverter.h"

#pragma once

namespace Util::FileIO {
    template <typename Endianness>
    class BinaryWriter : public BinaryIO<FileMode::BinaryWrite> {
    public:
        using BinaryIO::BinaryIO;

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
}
