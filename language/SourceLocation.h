#include <cstddef>
#include <util/StringView.h>

#pragma once

namespace LC3::Language {

struct SourceLocation {
    Util::StringView src;
    size_t lineNum = 0;
    size_t lineOffset = 0;
    size_t absOffset = 0;

    Util::StringView getLine() const {
        auto lineStart = src.begin() + absOffset;

        while (*lineStart != '\n' && lineStart != src.begin()) {
            --lineStart;
        }
        auto lineEnd = src.begin() + absOffset;

        while (*lineEnd != '\n' && lineEnd != src.end()) {
            ++lineEnd;
        }
        return { lineStart, lineEnd };
    }
};

} // namespace LC3::Language
