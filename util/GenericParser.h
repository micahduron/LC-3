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

        class SavepointGuard {
        public:
            SavepointGuard(const SavepointGuard& other) = delete;
            SavepointGuard(Context& context, bool defaultStatus = false) :
              m_context{ context },
              m_savepoint{ context.save() },
              m_status{ defaultStatus }
            {}

            ~SavepointGuard() {
                if (!m_status) {
                    m_context.restore(std::move(m_savepoint));
                }
            }

            SavepointGuard& operator = (const SavepointGuard& other) = delete;

            void setStatus(bool newStatus) {
                m_status = newStatus;
            }

            void save() {
                m_savepoint = m_context.save();
            }

            void restore() {
                m_context.restore(m_savepoint);
            }

        private:
            Context& m_context;
            typename Context::Savepoint m_savepoint;
            bool m_status;
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
                ParseState parseStatus = AttemptParse<ElemsH>(context);

                if constexpr (sizeof...(ElemsT) > 0) {
                    return parseStatus == ParseState::NonFatalFail ?
                        Any<ElemsT...>::parse(context) :
                        parseStatus;
                } else {
                    return parseStatus;
                }
            }
        };

private:
        template <typename ElemsH, typename... ElemsT>
        class All_Helper : public ParserElement {
        public:
            static ParseState parse(Context& context) {
                ParseState parseStatus = ElemsH::parse(context);

                if (parseStatus != ParseState::Success) {
                    return parseStatus;
                }
                if constexpr (sizeof...(ElemsT) > 0) {
                    return All_Helper<ElemsT...>::parse(context);
                } else {
                    return ParseState::Success;
                }
            }
        };

public:
        template <typename... Elems>
        class All : public ParserElement {
            static_assert( (GenericParser::template IsValidElement<Elems>() && ...) );

        public:
            static ParseState parse(Context& context) {
                SavepointGuard savepoint{ context };
                ParseState parseStatus = All_Helper<Elems...>::parse(context);

                savepoint.setStatus(parseStatus == ParseState::Success);

                return parseStatus;
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
                        ParseState parseStatus = AttemptParse<Elem>(context);

                        if (parseStatus != ParseState::Success) {
                            return count >= LowerBound ?
                                ParseState::Success :
                                ParseState::NonFatalFail;
                        }
                    }
                    return ParseState::Success;
                } else {
                    int count = 0;

                    while (AttemptParse<Elem>(context) == ParseState::Success) {
                        ++count;
                    }
                    return count >= LowerBound ?
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

    private:
        template <typename Elem>
        static ParseState AttemptParse(Context& context) {
            SavepointGuard savepoint{ context };
            ParseState parseStatus = Elem::parse(context);

            savepoint.setStatus(parseStatus == ParseState::Success);

            return parseStatus;
        }
    };
}
