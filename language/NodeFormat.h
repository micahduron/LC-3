#pragma once

namespace LC3::Language {

#define FORMATS \
    _(Empty) \
    _(Reg) \
    _(Num) \
    _(Str) \
    _(Addr) \
    _(Branch) \
    _(Vec) \
    _(NumNum) \
    _(NumAddr) \
    _(RegReg) \
    _(RegNum) \
    _(RegAddr) \
    _(RegRegReg) \
    _(RegRegNum) \
    _(RegRegAddr) \
    _(Invalid)

enum class NodeFormat {
#define _(FormatName) FormatName,
    FORMATS
#undef _
};

inline std::ostream& operator << (std::ostream& outStream, NodeFormat format) {
    switch (format) {
    #define _(FormatName) \
        case NodeFormat::FormatName: \
            outStream << #FormatName; \
            break;
        FORMATS
    #undef _
    }
    return outStream;
}

#undef FORMATS

} // namespace LC3::Language
