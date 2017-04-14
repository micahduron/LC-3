#pragma once

namespace Util {
    namespace Internals {
        enum FileModeEnum : int {
            // Basic file modes
            Read = 1 << 0,
            Write = 1 << 1,

            // Qualifiers
            Binary = 1 << 2,

            BinaryRead = Read | Binary,
            BinaryWrite = Write | Binary
        };

        template <typename Mode>
        class FileModeTraits_Helper {
            static constexpr bool IsBinary = Mode::ModeValue & FileModeEnum::Binary;
        };
    }

    namespace FileMode {
        using namespace Internals;

        class Read {
            static constexpr int ModeValue = FileModeEnum::Read;
        };
        class Write {
            static constexpr int ModeValue = FileModeEnum::Write;
        };
        class BinaryRead {
            static constexpr int ModeValue = FileModeEnum::BinaryRead;
        };
        class BinaryWrite {
            static constexpr int ModeValue = FileModeEnum::BinaryWrite;
        };
    }

    template <typename Mode>
    struct FileModeTraits {};

    template <>
    struct FileModeTraits<FileMode::Read> :
      public Internals::FileModeTraits_Helper<FileMode::Read>
    {
        static constexpr auto ModeString = "r";
    };
    template <>
    struct FileModeTraits<FileMode::Write> :
      public Internals::FileModeTraits_Helper<FileMode::Write>
    {
        static constexpr auto ModeString = "w";
    };
    template <>
    struct FileModeTraits<FileMode::BinaryRead> :
      public Internals::FileModeTraits_Helper<FileMode::BinaryRead>
    {
        static constexpr auto ModeString = "rb";
    };
    template <>
    struct FileModeTraits<FileMode::BinaryWrite> :
      public Internals::FileModeTraits_Helper<FileMode::BinaryWrite>
    {
        static constexpr auto ModeString = "wb";
    };
}
