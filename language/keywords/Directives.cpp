#include "Directives.h"

namespace LC3::Language::Keywords {

const KeywordMap<enum Directive> Directives::DirSet = {
    #define _(Name) { #Name ##_sv, Directive::Name },
    #include "Directives.str"
    #undef _
};

enum Directive Directives::get(const StringView& dirName) {
    auto setIter = DirSet.find(dirName);

    if (setIter != DirSet.end()) {
        return setIter->second;
    }
    return Directive::Invalid;
}

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
