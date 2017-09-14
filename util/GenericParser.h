#include <type_traits>
#include <utility>
#include "ParseState.h"
#include "ParserElementTraits.h"

#pragma once

namespace Util {
    template <typename Context>
    class GenericParser {
    public:
        using ContextType = Context;

        class ParserElement {
        public:
            using ContextType = Context;
            using ParentType = GenericParser;

            /** Required function definitions: **/
            // static ParseState parse(Context& context);
        };

        template <typename Elem>
        static constexpr bool IsValidElement() {
            return ParserElementTraits<Elem>::template IsCompatible<GenericParser>;
        }

        template <typename ElemsH, typename... ElemsT>
        class Any : public ParserElement {
            static_assert(GenericParser::template IsValidElement<ElemsH>());

        public:
            static ParseState parse(Context& context) {
                ParseState parseStatus = ElemsH::parse(context);

                if constexpr (sizeof...(ElemsT) > 0) {
                    return parseStatus == ParseState::NonFatalFail ?
                        Any<ElemsT...>::parse(context) :
                        parseStatus;
                } else {
                    return parseStatus;
                }
            }
        };

        template <typename ElemsH, typename... ElemsT>
        class All : public ParserElement {
            static_assert(GenericParser::template IsValidElement<ElemsH>());

        public:
            static ParseState parse(Context& context) {
                ParseState parseStatus = ElemsH::parse(context);

                if (parseStatus != ParseState::Success) {
                    return parseStatus;
                }
                if constexpr (sizeof...(ElemsT) > 0) {
                    return All<ElemsT...>::parse(context);
                } else {
                    return ParseState::Success;
                }
            }
        };

        template <typename Elem, int LowerBound = 0, int UpperBound = -1>
        class Multiple : public ParserElement {
            static_assert(GenericParser::template IsValidElement<Elem>());

            static_assert(LowerBound >= 0);
            static_assert(UpperBound <= 0 || LowerBound <= UpperBound);

        public:
            static ParseState parse(Context& context) {
                if constexpr (UpperBound >= 0) {
                    for (int count = 0; count < UpperBound; ++count) {
                        ParseState parseStatus = Elem::parse(context);

                        if (parseStatus == ParseState::FatalFail) {
                            return ParseState::FatalFail;
                        }
                        if (parseStatus != ParseState::Success) {
                            return count >= LowerBound ?
                                ParseState::Success :
                                parseStatus;
                        }
                    }
                    return ParseState::Success;
                } else {
                    int count = 0;
                    ParseState parseStatus = ParseState::FatalFail;

                    do {
                        parseStatus = Elem::parse(context);

                        if (parseStatus == ParseState::FatalFail) {
                            return ParseState::FatalFail;
                        }
                        ++count;
                    } while (parseStatus == ParseState::Success);

                    return (count >= LowerBound) ?
                        ParseState::Success :
                        ParseState::NonFatalFail;
                }
            }
        };

        // Convenient aliases for common special cases of the Multiple class.
        template <typename Elem, int LowerBound>
        class AtLeast : public Multiple<Elem, LowerBound, -1> {};

        template <typename Elem, int UpperBound>
        class AtMost : public Multiple<Elem, 0, UpperBound> {};

        template <typename Elem>
        class Maybe : public Multiple<Elem, 0, 1> {};

        template <typename Elem, int NumInstances>
        class Exactly : public Multiple<Elem, NumInstances, NumInstances> {};

        template <typename... Elems>
        class Many : public Multiple<All<Elems...>, 0, -1> {};

        template <typename Elem>
        class Many<Elem> : public Multiple<Elem, 0, -1> {};
    };
}
