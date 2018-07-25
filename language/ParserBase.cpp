#include <cassert>
#include <cstdlib>
#include <memory>
#include <string>
#include <util/StringView.h>
#include <util/StringUtils.h>
#include "TreeNodes.h"
#include "keywords/Instructions.h"
#include "keywords/Directives.h"
#include "ParserBase.h"

namespace LC3::Language {

using Util::StringView;
using Util::ParseState;
using Util::Str::CaselessCompare;
using Keywords::Instructions;
using Keywords::Instruction;
using Keywords::Directives;
using Keywords::Directive;

ParseState ParserBase::DirectiveName::parse(ParserContext& context) {
    assert(context.tree.treeTop() == context.tree.currRoot());

    Token token = *context.tokenizer;

    if (token.type == TokenType::Word) {
        Directive dirType = Directives::get(token.str);

        if (dirType != Directive::Invalid) {
            ++context.tokenizer;
            context.tree.descendTree<DirectiveNode>(dirType, token);

            return ParseState::Success;
        }
    }
    Log::error() << "Invalid directive name.\n"
                 << token.location.getLine() << '\n';

    return ParseState::FatalFail;
}

ParseState ParserBase::InstrName::parse(ParserContext& context) {
    assert(context.tree.treeTop() == context.tree.currRoot());

    Token token = *context.tokenizer;

    if (token.type == TokenType::Word) {
        Instruction instrType = Instructions::get(token.str);

        if (instrType == Instruction::Invalid) {
            if (context.flags & ErrorMode::Error) {
                Log::error() << token.location << " Unknown instruction "
                             << "name.\n";
                return ParseState::FatalFail;
            } else {
                return ParseState::NonFatalFail;
            }
        }
        ++context.tokenizer;
        context.tree.descendTree<InstructionNode>(InstructionData{instrType}, token);

        return ParseState::Success;
    }
    return ParseState::NonFatalFail;
}

ParseState ParserBase::LabelDefn::parse(ParserContext& context) {
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

void ParserBase::LabelDefn::ConstructNode(ParserContext& context, const Token& token) {
    SyntaxTreeNode& treeNode = context.tree.descendTree();

    treeNode.type = NodeType::LabelDefn;
    treeNode.token = token;

    context.tree.ascendTree();
}

ParseState ParserBase::LabelRef::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        if (context.flags & ErrorMode::Error) {
            Log::error() << token.location << " Unexpected token.\n"
                         << token.location.getLine() << '\n';

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

ParseState ParserBase::HexNumber::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        return ParseState::NonFatalFail;
    }
    if (!IsValid(token.str)) {
        return ParseState::NonFatalFail;
    }
    ++context.tokenizer;

    // Removes the leading 'x' character, leaving only the hex digits.
    token.str = token.str.subString(1);
    LC3::Word parsedNum = std::strtoul(token.str.data(), nullptr, 16);
    context.tree.descendTree<NumberNode>(parsedNum, token);

    return ParseState::Success;
}

bool ParserBase::HexNumber::IsValid(const StringView& tokenStr) {
    if (tokenStr.size() < 2) {
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

ParseState ParserBase::DecNumberDefn::parse(ParserContext& context) {
    Token token = *context.tokenizer;
    bool isNegative = false;

    if (token.type == TokenType::Minus) {
        isNegative = true;
        token = *++context.tokenizer;
    }
    if (token.type != TokenType::Number) {
        if (context.flags & ErrorMode::Error) {
            Log::error() << "Expected number\n";

            return ParseState::FatalFail;
        } else {
            return ParseState::NonFatalFail;
        }
    }
    if (!DecNumberDefn::IsValid(token.str)) {
        if (context.flags & ErrorMode::Error) {
            Log::error() << "Invalid number\n";

            return ParseState::FatalFail;
        } else {
            return ParseState::NonFatalFail;
        }
    }
    ++context.tokenizer;

    LC3::Word parsedNum = std::strtoul(token.str.data(), nullptr, 10);

    if (isNegative) {
        parsedNum = ~parsedNum + 1;
    }
    context.tree.descendTree<NumberNode>(parsedNum, token);

    return ParseState::Success;
}

bool ParserBase::DecNumberDefn::IsValid(const StringView& tokenStr) {
    if (tokenStr.size() == 0) return false;

    return std::all_of(tokenStr.begin(), tokenStr.end(), [](char c) -> bool
    {
        auto cu = static_cast<unsigned char>(c);

        return std::isdigit(cu) != 0;
    });
}

static std::string GetString(const StringView& tokenStr) {
    std::string result;
    result.reserve(tokenStr.size());

    size_t i = 0;
    size_t k = 0;

    for (; i < tokenStr.size(); ++i, ++k) {
        char currChar = '\0';

        if (tokenStr[i] == '\\') {
            char nextChar = tokenStr[i + 1];

            if (nextChar == 'n') {
                currChar = '\n';
            } else {
                currChar = nextChar;
            }
            ++i;
        } else {
            currChar = tokenStr[i];
        }
        result += currChar;
    }
    return result;
}

ParseState ParserBase::String::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::String) {
        return ParseState::NonFatalFail;
    }
    ++context.tokenizer;

    auto strVal = std::make_shared<std::string>(GetString(token.str));
    context.tree.descendTree<StringNode>(std::move(strVal), token);

    return ParseState::Success;
}

ParseState ParserBase::Register::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        return ParseState::NonFatalFail;
    }
    if (token.str.size() != 2) {
        return ParseState::NonFatalFail;
    }
    if (token.str.beginsWith("R", CaselessCompare()) &&
        (token.str[1] >= '0' && token.str[1] <= '7'))
    {
        ++context.tokenizer;

        LC3::Word regNum = token.str[1] - '0';
        context.tree.descendTree<RegisterNode>(regNum, token);

        return ParseState::Success;
    }
    return ParseState::NonFatalFail;
}

} // namespace LC3::Language

