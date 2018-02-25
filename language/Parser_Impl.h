#include <algorithm>
#include <cassert>
#include <cctype>
#include <util/GenericParser.h>
#include "Token.h"
#include "Tokenizer.h"
#include "ParserContext.h"
#include "Constants.h"

#pragma once

namespace LC3::Language {

struct Parser_Impl : protected Util::GenericParser<ParserContext> {
    enum class FailureMode {
        Ignore,
        Fatal,
        Warn
    };

    using ParseState = Util::ParseState;

    // This helper element will keep track of the syntax tree's position
    // and will return to it once the supplied elements have been executed.
    template <typename... Elems>
    struct TreeChild : public ParserElement {
        static ParseState parse(ParserContext& context) {
            SyntaxTreeBuilder::DescentGuard guard(context.tree);

            return All<Elems...>::parse(context);
        }
    };

    // An atom verifies if the current token is of an expected type and will
    // consume it if it matches, otherwise it will do nothing. This element
    // does not modify the AST in any way.
    template <TokenType ExpectedType>
    struct Atom : public ParserElement {
        static ParseState parse(ParserContext& context) {
            Token token = *context.tokenizer;

            if (token.type == ExpectedType) {
                ++context.tokenizer;

                return ParseState::Success;
            }
            if (context.flags & ErrorMode::Error) {
                context.log.error() << token.location << " Unexpected token '"
                                    << token.type << "'.\n"
                                    << token.location.getLine() << '\n';
                return ParseState::FatalFail;
            }
            return ParseState::NonFatalFail;
        }
    };

    template <typename FlagHandler, typename FlagHandler::value FlagValue, typename Elem>
    struct SetFlags : public ParserElement {
        static ParseState parse(ParserContext& context) {
            ParserFlags& parseFlags = context.flags;

            auto prevFlag = FlagHandler::get(parseFlags);
            FlagHandler::set(parseFlags, FlagValue);

            ParseState status = Elem::parse(context);

            FlagHandler::set(parseFlags, prevFlag);

            return status;
        }
    };

    template <typename ParserFlags::ErrorFlags::value ErrMode, typename Elem>
    using SetErrorMode = SetFlags<ParserFlags::ErrorFlags, ErrMode, Elem>;

    template <typename Elem>
    using SetError = SetErrorMode<ErrorMode::Error, Elem>;

    template <typename Elem>
    using SetIgnore = SetErrorMode<ErrorMode::Ignore, Elem>;

    template <typename Elem>
    using SetWarn = SetErrorMode<ErrorMode::Warn, Elem>;

    template <typename Elem>
    using SetDefault = SetIgnore<Elem>;

    template <typename Elem>
    struct HaltOnFail : public ParserElement {
        static ParseState parse(ParserContext& context) {
            ParseState status = SetError<Elem>::parse(context);

            return status != ParseState::Success ?
                ParseState::FatalFail :
                ParseState::Success;
        }
    };

    template <typename ElemsHead, typename... ElemsTail>
    class HaltIfNone_Sub : public Any<ElemsHead, HaltIfNone_Sub<ElemsTail...>> {};

    template <typename ElemLast>
    class HaltIfNone_Sub<ElemLast> : public HaltOnFail<ElemLast> {};

    template <typename... Elems>
    using HaltIfNone = SetDefault<HaltIfNone_Sub<Elems...>>;

    struct DirectiveName : public ParserElement {
        static ParseState parse(ParserContext& context) {
            assert(context.tree.treeTop() == context.tree.currRoot());

            Token token = *context.tokenizer;

            if (token.type == TokenType::Word && Constants::IsDirective(token.str)) {
                ++context.tokenizer;

                SyntaxTreeNode& newRoot = context.tree.descendTree();

                newRoot.type = NodeType::Directive;
                newRoot.token = token;

                return ParseState::Success;
            }
            context.log.error() << "Invalid directive name.\n";

            return ParseState::FatalFail;
        }
    };

    struct InstrName : public ParserElement {
        static ParseState parse(ParserContext& context) {
            assert(context.tree.treeTop() == context.tree.currRoot());

            Token token = *context.tokenizer;

            if (token.type == TokenType::Word) {
                if (!Constants::IsInstruction(token.str)) {
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
    };

    struct LabelDefn : public ParserElement {
        static ParseState parse(ParserContext& context) {
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
            } else if (!Constants::IsReserved(token.str)) {
                ++context.tokenizer;

                ConstructNode(context, token);

                return ParseState::Success;
            }
            return ParseState::NonFatalFail;
        }
    private:
        static void ConstructNode(ParserContext& context, const Token& token) {
            SyntaxTreeNode& treeNode = context.tree.descendTree();

            treeNode.type = NodeType::LabelDefn;
            treeNode.token = token;

            context.tree.ascendTree();
        }
    };

    struct LabelRef : public ParserElement {
        static ParseState parse(ParserContext& context) {
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
    };

    struct HexNumber : public ParserElement {
        static ParseState parse(ParserContext& context) {
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

    private:
        static bool IsValid(const Util::StringView& tokenStr) {
            auto startIter = tokenStr.begin();
            auto endIter = tokenStr.end();

            if (startIter == endIter) {
                return false;
            }
            if (std::tolower(*startIter) != 'x') {
                return false;
            }
            return std::all_of(startIter + 1, endIter, [](char c) -> bool {
                auto cu = static_cast<unsigned char>(c);

                return std::isxdigit(cu) != 0;
            });
        }
    };

    struct DecNumberDefn : public ParserElement {
        static ParseState parse(ParserContext& context) {
            Token token = *context.tokenizer;

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

    private:
        static bool IsValid(const Util::StringView& tokenStr) {
            return std::all_of(tokenStr.begin(), tokenStr.end(), [](char c) -> bool {
                auto cu = static_cast<unsigned char>(c);

                return std::isdigit(cu) != 0;
            });
        }
    };

    struct String : public ParserElement {
        static ParseState parse(ParserContext& context) {
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
    };

    struct Register : public ParserElement {
        static ParseState parse(ParserContext& context) {
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
    };
};

} // namespace LC3::Language
