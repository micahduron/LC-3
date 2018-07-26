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

static bool IsBranchInstruction(const StringView& strVal) {
    if (!strVal.beginsWith("BR", CaselessCompare())) {
        return false;
    }
    for (char c : strVal.subString(2)) {
        char lowC = std::tolower(c);

        if (!(lowC == 'n' || lowC == 'z' || lowC == 'p')) {
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
