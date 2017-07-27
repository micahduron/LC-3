#include <type_traits>
#include "Endianness.h"

#pragma once

namespace Util {
    namespace Internals {
        template <typename T>
        T SwitchEndianness(T value) {
            static_assert(std::is_unsigned<T>::value == true,
                          "Requires unsigned integral type."
            );
            T result = 0;

            for (size_t i = 0; i < sizeof(T); ++i) {
                T lowestByte = value & 0xFF;

                result = (result << 8) | lowestByte;
                value >>= 8;
            }
            return result;
        }

        template <typename T>
        T KeepEndianness(T value) {
            static_assert(std::is_unsigned<T>::value == true,
                          "Requires unsigned integral type."
            );
            return value;
        }

        struct SameEndianness {
            template <typename T>
            static T Encode(T value) {
                return KeepEndianness(value);
            }
            template <typename T>
            static T Decode(T value) {
                return KeepEndianness(value);
            }
        };

        struct DifferentEndianness {
            template <typename T>
            static T Encode(T value) {
                return SwitchEndianness(value);
            }
            template <typename T>
            static T Decode(T value) {
                return SwitchEndianness(value);
            }
        };
    }

    template <int Endianness>
    class EndiannessConverter {};

    template <>
    class EndiannessConverter<LittleEndian> :
      public std::conditional<
        SystemEndianness == LittleEndian,
        Internals::SameEndianness,
        Internals::DifferentEndianness
      >::type
    {};

    template <>
    class EndiannessConverter<BigEndian> :
      public std::conditional<
        SystemEndianness == BigEndian,
        Internals::SameEndianness,
        Internals::DifferentEndianness
      >::type
    {};

    template <>
    class EndiannessConverter<SameEndianness> :
      public Internals::SameEndianness
    {};

    template <>
    class EndiannessConverter<OppositeEndianness> :
      public Internals::DifferentEndianness
    {};
}
