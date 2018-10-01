#pragma once

#include <iostream>
#include <lc3/Word.h>
#include <util/StringView.h>
#include <util/KeywordMap.h>

namespace LC3::Language::Keywords {

using Util::StringView;
using Util::KeywordMap;

enum class Instruction : LC3::Word::value_type {
    #define _(Name, Opcode) Name = Opcode,
    #include "Instructions.str"
    #undef _

    Invalid = 0xFFFF
};

class Instructions {
    static const KeywordMap<enum Instruction> InstrMap;

public:
    static bool has(const StringView& instrName) {
        return get(instrName) != Instruction::Invalid;
    }

    static enum Instruction get(const StringView& instrName);

    static LC3::Word getOpcode(const StringView& instrName);
    static LC3::Word getOpcode(Instruction instr);
};

std::ostream& operator << (std::ostream& outStream, Instruction instr);

} // namespace LC3::Language::Keywords
