#pragma once

#include <cctype>
#include <util/StringView.h>

namespace Util::Str {

struct CaselessHash {
    size_t operator () (const StringView& strView) const {
        return StringView::hash(strView, [](char c) -> char {
            return std::toupper(static_cast<unsigned char>(c));
        });
    }
};

struct CaselessCompare {
    int operator () (char c1, char c2) const {
        int c1Upper = std::toupper(static_cast<unsigned char>(c1));
        int c2Upper = std::toupper(static_cast<unsigned char>(c2));

        return c1Upper - c2Upper;
    }
};

struct CaselessEqual {
    bool operator () (const StringView& str1, const StringView& str2) const {
        return str1.compare(str2, CaselessCompare()) == 0;
    }
};

} // namespace Util::Str
