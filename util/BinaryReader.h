#include <cstdio>
#include <type_traits>
#include "BinaryIO.h"
#include "FileMode.h"

#pragma once

namespace Util {
    template <typename Endianness>
    class BinaryReader : public BinaryIO<FileMode::BinaryRead> {
    public:
        using BinaryIO::BinaryIO;

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

            if (std::fread(&datum, sizeof(T), 1, file()) != 1) {
                return false;
            }
            outputRef = Endianness::Decode(datum);

            return true;
        }
    };
}
