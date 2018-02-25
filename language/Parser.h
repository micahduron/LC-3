#include <optional>
#include "Parser_Impl.h"
#include "Token.h"
#include "ParserContext.h"

#pragma once

namespace LC3::Language {

class Parser : protected Parser_Impl {
protected:
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

    using InstrArg = TreeChild<Any<Register, Number, LabelRef>>;
    using InstrArgList = All<InstrArg, Many<Comma, InstrArg>>;
    using Instruction = TreeChild<InstrName, InstrArgList>;

    using Line = All<Maybe<LabelDefn>, Maybe<Any<Instruction, Directive>>, Linebreak>;
    using Document = All<Many<Line>, End>;

public:
    std::optional<SyntaxTreeNode> parse(Util::StringView src) const {
        ParserContext context(src);

        if (Document::parse(context) != ParseState::Success) {
            return {};
        }
        return { std::move(context.tree.treeTop()) };
    }

    std::optional<SyntaxTreeNode> operator() (Util::StringView src) const {
        return parse(src);
    }
};

} // namespace LC3::Language
