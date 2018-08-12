#include <cassert>
#include <cstdlib>
#include <exception>
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
    Log::error(token) << "Invalid directive name.\n";

    return ParseState::FatalFail;
}

static BRFlagsNode MakeBranchFlags(const Token& flagToken) {
    BRFlagsData branchFlags;

    for (char c : flagToken.str) {
        switch (std::tolower(c)) {
            case 'n':
                branchFlags.n = true;
                break;
            case 'z':
                branchFlags.z = true;
                break;
            case 'p':
                branchFlags.p = true;
                break;
            default:
                throw std::logic_error("Unimplemented branch flag");
        }
    }
    return BRFlagsNode(std::move(branchFlags), flagToken);
}

ParseState ParserBase::InstrName::parse(ParserContext& context) {
    assert(context.tree.treeTop() == context.tree.currRoot());

    Token token = *context.tokenizer;

    if (token.type == TokenType::Word) {
        Instruction instrType = Instructions::get(token.str);

        if (instrType == Instruction::Invalid) {
            if (context.flags & ErrorMode::Error) {
                Log::error(token) << "Unknown instruction.\n";

                return ParseState::FatalFail;
            } else {
                return ParseState::NonFatalFail;
            }
        }
        ++context.tokenizer;

        if (instrType == Instruction::BR) {
            Token flagsToken = token;
            flagsToken.str = token.str.subString(2);

            token.str = token.str.subString(0, 2);

            auto flagsNode = MakeBranchFlags(flagsToken);

            auto& instrNode = context.tree.descendTree<InstructionNode>(
                                    InstructionData{instrType}, token);
            instrNode.children.push_back(std::move(flagsNode));
        } else {
            context.tree.descendTree<InstructionNode>(InstructionData{instrType},
                                                      token);
        }
        return ParseState::Success;
    }
    return ParseState::NonFatalFail;
}

ParseState ParserBase::LabelDefn::parse(ParserContext& context) {
    assert(context.tree.treeTop() == context.tree.currRoot());

    SyntaxTreeBuilder::DescentGuard guard{ context.tree };
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        return ParseState::NonFatalFail;
    }
    Token nextToken = context.tokenizer.peek(1);

    // A strange quirk of LC-3 is that labels can be reserved words,
    // even valid hexadecimal numbers, if they are followed by a colon.
    if (nextToken.type == TokenType::Colon) {
        context.tokenizer += 2;
    
        context.tree.descendTree(NodeType::LabelDefn, token);

        return ParseState::Success;
    } else if (!Instructions::has(token.str) && !Directives::has(token.str)) {
        ++context.tokenizer;

        context.tree.descendTree(NodeType::LabelDefn, token);

        return ParseState::Success;
    }
    return ParseState::NonFatalFail;
}

ParseState ParserBase::LabelRef::parse(ParserContext& context) {
    Token token = *context.tokenizer;

    if (token.type != TokenType::Word) {
        if (context.flags & ErrorMode::Error) {
            Log::error(token) << "Unexpected " << token.type << " token.\n";

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
            Log::error(token) << "Expected number but got " << token.type << ".\n";

            return ParseState::FatalFail;
        } else {
            return ParseState::NonFatalFail;
        }
    }
    if (!DecNumberDefn::IsValid(token.str)) {
        if (context.flags & ErrorMode::Error) {
            Log::error() << "Invalid number format.\n";

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

