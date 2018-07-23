#pragma once

#include "ParserBase.h"
#include "Token.h"

namespace LC3::Language {

struct Grammar : public ParserBase {
    using Period = Atom<TokenType::Period>;
    using Comma = Atom<TokenType::Comma>;
    using PoundSign = Atom<TokenType::Pound>;
    using Colon = Atom<TokenType::Colon>;
    using Minus = Atom<TokenType::Minus>;
    using Linebreak = Atom<TokenType::Linebreak>;
    using End = SetError<Atom<TokenType::End>>;

    using DecimalNumber = Any<
        All<PoundSign, SetError<DecNumberDefn>>,
        DecNumberDefn
    >;
    using Number = Any<HexNumber, DecimalNumber>;

    using DirectiveArg = TreeChild<Any<Number, LabelRef, String>>;
    using DirectiveStmt = TreeChild<DirectiveName, Many<DirectiveArg>>;
    using Directive = All<Period, DirectiveStmt>;

    template <template <typename... Ts> typename DisjuncType>
    using InstrArg_T = TreeChild<DisjuncType<Register, Number, LabelRef>>;

    using InstrArg_Head = InstrArg_T<Any>;
    using InstrArg_Tail = InstrArg_T<HaltIfNone>;

    using InstrArgList = Maybe<All<InstrArg_Head, Many<Comma, InstrArg_Tail>>>;
    using Instruction = TreeChild<SetError<InstrName>, InstrArgList>;

    using Line = All<
        Maybe<LabelDefn>,
        Maybe<
          Any<
            Directive,
            Instruction
          >
        >,
        Linebreak
    >;
    using Document = All<Many<Line>, End>;
};

} // namespace LC3::Language
