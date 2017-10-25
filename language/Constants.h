#include <iostream>
#include <unordered_set>
#include <cctype>
#include <util/StringView.h>

#pragma once

namespace LC3::Language {

class Constants {
    using StringView = Util::StringView;

    struct CaselessHash {
        size_t operator () (const StringView& strView) const {
            return StringView::hash(strView, [](char c) -> char {
                return std::toupper(static_cast<unsigned char>(c));
            });
        }
    };

    struct CaselessCmp {
        int operator () (char c1, char c2) const {
            int c1Upper = std::toupper(static_cast<unsigned char>(c1));
            int c2Upper = std::toupper(static_cast<unsigned char>(c2));

            return c1Upper - c2Upper;
        }
    };

    struct CaselessEqual {
        bool operator () (const StringView& str1, const StringView& str2) const {
            return str1.compare(str2, CaselessCmp()) == 0;
        }
    };

    using BaseStringSet = std::unordered_set<
      StringView,
      CaselessHash,
      CaselessEqual
    >;

    class StringSet : private BaseStringSet {
    public:
        using BaseStringSet::BaseStringSet;

        bool has(const StringView& str) const {
            return find(str) != end();
        }
    };

    static inline const StringSet Directives = {
        #define _(x) #x ##_sv,

        #include "./keywords/Directives.str"

        #undef _
    };
    static inline const StringSet Instructions = {
        #define _(x) #x ##_sv,

        #include "./keywords/Instructions.str"

        #undef _
    };

public:
    static bool IsDirective(const StringView& strView) {
        return Directives.has(strView);
    }

    static bool IsInstruction(const StringView& strView) {
        // The BR instruction is a special case in that it can have
        // user-specified suffixes.
        return Instructions.has(strView) || strView.beginsWith("BR"_sv, CaselessCmp());
    }
    
    static bool IsReserved(const StringView& strView) {
        return IsDirective(strView) || IsInstruction(strView);
    }
};

} // namespace LC3::Language
