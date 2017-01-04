#include <type_traits>
#include "Endianness.h"

#pragma once

namespace Util::Endianness::Converter {
    namespace Internal {
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
            static T encode(T value) {
                return KeepEndianness(value);
            }
            template <typename T>
            static T decode(T value) {
                return KeepEndianness(value);
            }
        };

        struct DifferentEndianness {
            template <typename T>
            static T encode(T value) {
                return SwitchEndianness(value);
            }
            template <typename T>
            static T decode(T value) {
                return SwitchEndianness(value);
            }
        };
    }
    using NullConverter = Internal::SameEndianness;

    using LittleEndian = std::conditional<
      GetSystemEndianness() == Little,
        struct Internal::SameEndianness,
        struct Internal::DifferentEndianness
    >::type;

    using BigEndian = std::conditional<
      GetSystemEndianness() == Big,
        struct Internal::SameEndianness,
        struct Internal::DifferentEndianness
    >::type;

    static_assert(std::is_same<LittleEndian, BigEndian>::value == false,
                  "LittleEndian is not supposed to be the same type as BigEndian."
    );
}
