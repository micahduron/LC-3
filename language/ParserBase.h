#pragma once

#include <Log.h>
#include <util/GenericParser.h>
#include "Token.h"
#include "Tokenizer.h"
#include "ParserContext.h"

namespace LC3::Language {

class ParserBase : public Util::GenericParser<ParserContext> {
public:
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
                Log::error(token) << "Unexpected " << token.type << " token.\n";

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
        static ParseState parse(ParserContext& context);
    };

    struct InstrName : public ParserElement {
        static ParseState parse(ParserContext& context);
    };

    struct LabelDefn : public ParserElement {
        static ParseState parse(ParserContext& context);

    private:
        static void ConstructNode(ParserContext& context, const Token& token);
    };

    struct LabelRef : public ParserElement {
        static ParseState parse(ParserContext& context);
    };

    struct HexNumber : public ParserElement {
        static ParseState parse(ParserContext& context);

    private:
        static bool IsValid(const Util::StringView& tokenStr);
    };

    struct DecNumberDefn : public ParserElement {
        static ParseState parse(ParserContext& context);

    private:
        static bool IsValid(const Util::StringView& tokenStr);
    };

    struct String : public ParserElement {
        static ParseState parse(ParserContext& context);
    };

    struct Register : public ParserElement {
        static ParseState parse(ParserContext& context);
    };
};

} // namespace LC3::Language
