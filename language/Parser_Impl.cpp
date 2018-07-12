#include <cassert>
#include <util/StringView.h>
#include "keywords/Instructions.h"
#include "keywords/Directives.h"
#include "Parser_Impl.h"

namespace LC3::Language {

using Util::StringView;
using Util::ParseState;
using Keywords::Instructions;
using Keywords::Directives;

ParseState Parser_Impl::DirectiveName::parse(ParserContext& context) {
    assert(context.tree.treeTop() == context.tree.currRoot());

    Token token = *context.tokenizer;

    if (token.type == TokenType::Word && Directives::has(token.str)) {
        ++context.tokenizer;

        SyntaxTreeNode& newRoot = context.tree.descendTree();

        newRoot.type = NodeType::Directive;
        newRoot.token = token;

        return ParseState::Success;
    }
    context.log.error() << "Invalid directive name.\n"
                        << token.location.getLine() << '\n';

    return ParseState::FatalFail;
}

ParseState Parser_Impl::InstrName::parse(ParserContext& context) {
    assert(context.tree.treeTop() == context.tree.currRoot());

    Token token = *context.tokenizer;

    if (token.type == TokenType::Word) {
        if (!Instructions::has(token.str)) {
            if (context.flags & ErrorMode::Error) {
                context.log.error() << token.location << " Unknown instruction "
                                    << "name.\n";
                return ParseState::FatalFail;
            } else {
                return ParseState::NonFatalFail;
            }
        }
        ++context.tokenizer;

        SyntaxTreeNode& treeNode = context.tree.descendTree();

        treeNode.type = NodeType::Instruction;
        treeNode.token = token;

        return ParseState::Success;
    }
    return ParseState::NonFatalFail;
}

ParseState Parser_Impl::LabelDefn::parse(ParserContext& context) {
    assert(context.tree.treeTop() == context.tree.currRoot());

    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        return ParseState::NonFatalFail;
    }
    Token nextToken = context.tokenizer.peek(1);

    // A strange quirk of LC-3 is that labels can be reserved words,
    // even valid hexadecimal numbers, if they are followed by a colon.
    if (nextToken.type == TokenType::Colon) {
        context.tokenizer += 2;
    
        ConstructNode(context, token);

        return ParseState::Success;
    } else if (!Instructions::has(token.str) && !Directives::has(token.str)) {
        ++context.tokenizer;

        ConstructNode(context, token);

        return ParseState::Success;
    }
    return ParseState::NonFatalFail;
}

void Parser_Impl::LabelDefn::ConstructNode(ParserContext& context, const Token& token) {
    SyntaxTreeNode& treeNode = context.tree.descendTree();

    treeNode.type = NodeType::LabelDefn;
    treeNode.token = token;

    context.tree.ascendTree();
}

ParseState Parser_Impl::LabelRef::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        if (context.flags & ErrorMode::Error) {
            context.log.error() << token.location << " Unexpected token.\n";
            context.log.error() << token.location.getLine() << '\n';

            return ParseState::FatalFail;
        }
        return ParseState::NonFatalFail;
    }
    ++context.tokenizer;

    SyntaxTreeNode& treeNode = context.tree.descendTree();

    treeNode.type = NodeType::LabelRef;
    treeNode.token = std::move(token);

    return ParseState::Success;
}

ParseState Parser_Impl::HexNumber::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        return ParseState::NonFatalFail;
    }
    if (!IsValid(token.str)) {
        return ParseState::NonFatalFail;
    }
    ++context.tokenizer;

    SyntaxTreeNode& treeNode = context.tree.descendTree();

    treeNode.type = NodeType::HexNumber;

    // Removes the leading 'x' character, leaving only the hex digits.
    token.str = token.str.subString(1, token.str.size() - 1);
    treeNode.token = std::move(token);

    return ParseState::Success;
}

bool Parser_Impl::HexNumber::IsValid(const StringView& tokenStr) {
    if (tokenStr.size() == 0) {
        return false;
    }
    auto startIter = tokenStr.begin();
    auto endIter = tokenStr.end();

    if (std::tolower(*startIter) != 'x') {
        return false;
    }
    return std::all_of(startIter + 1, endIter, [](char c) -> bool {
        auto cu = static_cast<unsigned char>(c);

        return std::isxdigit(cu) != 0;
    });
}

ParseState Parser_Impl::DecNumberDefn::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type == TokenType::Minus) {
        SyntaxTreeNode& node = context.tree.descendTree();

        node.type = NodeType::NegNumber;
        node.token = token;

        token = *++context.tokenizer;
    }
    if (token.type != TokenType::Number) {
        if (context.flags & ErrorMode::Error) {
            context.log.error() << "Expected number\n";

            return ParseState::FatalFail;
        } else {
            return ParseState::NonFatalFail;
        }
    }
    if (!DecNumberDefn::IsValid(token.str)) {
        if (context.flags & ErrorMode::Error) {
            context.log.error() << "Invalid number\n";

            return ParseState::FatalFail;
        } else {
            return ParseState::NonFatalFail;
        }
    }
    ++context.tokenizer;

    SyntaxTreeNode& treeNode = context.tree.descendTree();

    treeNode.type = NodeType::DecNumber;
    treeNode.token = token;

    return ParseState::Success;
}

bool Parser_Impl::DecNumberDefn::IsValid(const StringView& tokenStr) {
    if (tokenStr.size() == 0) return false;

    return std::all_of(tokenStr.begin(), tokenStr.end(), [](char c) -> bool
    {
        auto cu = static_cast<unsigned char>(c);

        return std::isdigit(cu) != 0;
    });
}

ParseState Parser_Impl::String::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::String) {
        return ParseState::NonFatalFail;
    }
    ++context.tokenizer;

    SyntaxTreeNode& treeNode = context.tree.descendTree();

    treeNode.type = NodeType::String;
    treeNode.token = std::move(token);

    return ParseState::Success;
}

ParseState Parser_Impl::Register::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        return ParseState::NonFatalFail;
    }
    if (token.str.size() != 2) {
        return ParseState::NonFatalFail;
    }
    if ((token.str.beginsWith("R"_sv) || token.str.beginsWith("r"_sv)) &&
        (token.str[1] >= '0' && token.str[1] <= '7'))
    {
        ++context.tokenizer;

        SyntaxTreeNode& treeNode = context.tree.descendTree();

        treeNode.type = NodeType::Register;
        treeNode.token = std::move(token);

        return ParseState::Success;
    }
    return ParseState::NonFatalFail;
}

} // namespace LC3::Language
