#pragma once

#include <lc3.h>
#include <util/StringView.h>
#include <util/StringUtils.h>
#include <util/KeywordMap.h>

namespace LC3::Language::Keywords {

using Util::StringView;
using Util::KeywordMap;
using Util::Str::CaselessCompare;

enum class Instruction : LC3::Word {
    #define _(Name, Opcode) Name = Opcode,
    #include "Instructions.str"
    #undef _

    Invalid = 0xFFFF
};

class Instructions {
    static inline const KeywordMap<enum Instruction> InstrMap = {
        #define _(Name, Opcode) { #Name ##_sv, Instruction::Name },
        #include "Instructions.str"
        #undef _
    };

public:
    static bool has(const StringView& instrName) {
        return get(instrName) != Instruction::Invalid;
    }

    static enum Instruction get(const StringView& instrName) {
        auto mapIter = InstrMap.find(instrName);

        if (mapIter == InstrMap.end()) {
            // A special case in the LC3 grammar is the BR instruction.
            // The BR instruction has a variable number of flags that
            // are specified in the instruction's name.
            return instrName.beginsWith("BR", CaselessCompare()) ?
                Instruction::BR :
                Instruction::Invalid;
        }
        return mapIter->second;
    }

    static LC3::Word getOpcode(const StringView& instrName) {
        return static_cast<LC3::Word>(get(instrName));
    }

    static LC3::Word getOpcode(Instruction instr) {
        return static_cast<LC3::Word>(instr);
    }
};

} // namespace LC3::Language::Keywords
