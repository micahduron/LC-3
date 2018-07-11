#pragma once

#include <iostream>
#include <lc3.h>
#include <util/StringView.h>
#include <util/KeywordMap.h>

namespace LC3::Language::Keywords {

using Util::StringView;
using Util::KeywordMap;

enum class Directive {
    #define _(Name) Name,
    #include "Directives.str"

    _(Invalid)
    #undef _
};

class Directives {
    static inline const KeywordMap<enum Directive> DirSet = {
        #define _(Name) { #Name ## _sv, Directive::Name },
        #include "Directives.str"
        #undef _
    };

public:
    static bool has(const StringView& dirName) {
        return get(dirName) != Directive::Invalid;
    }

    static enum Directive get(const StringView& dirName) {
        auto setIter = DirSet.find(dirName);

        if (setIter != DirSet.end()) {
            return setIter->second;
        }
        return Directive::Invalid;
    }
};

std::ostream& operator << (std::ostream& outStream, Directive dir) {
    switch (dir) {
        #define _(Name) \
            case Directive::Name: \
                outStream << #Name; \
                break;
        #include "Directives.str"

        _(Invalid)
        #undef _
    }
    return outStream;
}

} // namespace LC3::Language::Keywords
