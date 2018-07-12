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
    static const KeywordMap<enum Directive> DirSet;

public:
    static bool has(const StringView& dirName) {
        return get(dirName) != Directive::Invalid;
    }

    static enum Directive get(const StringView& dirName);
};

std::ostream& operator << (std::ostream& outStream, Directive dir);

} // namespace LC3::Language::Keywords
