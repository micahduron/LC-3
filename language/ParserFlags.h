#include <cstddef>

#pragma once

namespace LC3::Language {

enum class ErrorMode {
    Default,
    Ignore,
    Warn,
    Error
};

struct ParserFlags {
    struct ErrorFlags {
        using value = ErrorMode;

        static value get(const ParserFlags& parseFlags) {
            return parseFlags.errorFlags;
        }

        static void set(ParserFlags& parseFlags, value flagVal) {
            parseFlags.errorFlags = flagVal;
        }

    private:
        value m_value;
    };

    ErrorFlags::value errorFlags = ErrorMode::Default;

    bool operator & (ErrorMode errMode) const {
        return errorFlags == errMode;
    }
};

} // namespace LC3::Language
