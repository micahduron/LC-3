#include <cctype>
#include <util/StringUtils.h>
#include "Instructions.h"

namespace LC3::Language::Keywords {

using Util::StringView;
using Util::Str::CaselessCompare;

const KeywordMap<enum Instruction> Instructions::InstrMap = {
    #define _(Name, Opcode) { #Name ##_sv, Instruction::Name },
    #include "Instructions.str"
    #undef _
};

struct BranchFlags {
    bool n = false;
    bool z = false;
    bool p = false;
};

static bool IsBranchInstruction(const StringView& strVal) {
    if (!strVal.beginsWith("BR", CaselessCompare())) {
        return false;
    }
    BranchFlags seenFlags;

    for (char c : strVal.subString(2)) {
        switch (std::tolower(c)) {
            case 'n':
                if (seenFlags.n) {
                    return false;
                }
                seenFlags.n = true;
                break;
            case 'z':
                if (seenFlags.z) {
                    return false;
                }
                seenFlags.z = true;
                break;
            case 'p':
                if (seenFlags.p) {
                    return false;
                }
                seenFlags.p = true;
                break;
            default:
                return false;
        }
    }
    return true;
}

enum Instruction Instructions::get(const StringView& instrName) {
    auto mapIter = InstrMap.find(instrName);

    if (mapIter == InstrMap.end()) {
        // A special case in the LC3 grammar is the BR instruction.
        // The BR instruction has a variable number of flags that
        // are specified in the instruction's name.
        return IsBranchInstruction(instrName) ?
            Instruction::BR :
            Instruction::Invalid;
    }
    return mapIter->second;
}

LC3::Word Instructions::getOpcode(const StringView& instrName) {
    Instruction instrVal = Instructions::get(instrName);

    return Instructions::getOpcode(instrVal);
}

LC3::Word Instructions::getOpcode(Instruction instr) {
    return LC3::Word(static_cast<LC3::Word::value_type>(instr));
}

std::ostream& operator << (std::ostream& outStream, Instruction instr) {
    switch (instr) {
        #define _(Name, Opcode) \
            case Instruction::Name: \
                outStream << #Name; \
                break;
        #include "Instructions.str"

        _(Invalid, 0xFFFF)
        #undef _
    }
    return outStream;
}

} // namespace LC3::Language::Keywords
