#include <type_traits>
#include <utility>

#pragma once

#define TemplateString(ClassName, StringVal) struct ClassName { static constexpr auto value = StringVal; }

namespace Util {
    template <typename Context>
    class GenericParser {
    public:
        class ParserElement {
        public:
            using context = Context;

            /** Required function definitions: **/
            // static bool parse(Context& context);
        };

    private:
        template <typename Elem>
        class IsValidElement :
          public std::conditional<
            std::is_base_of<ParserElement, Elem>::value &&
              std::is_same<decltype(Elem::parse(std::declval<Context&>())), bool>::value,
            std::true_type,
            std::false_type
          >::type
        {};

    public:
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
                    m_context.restore(m_savepoint);
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
            typename Context::savepoint m_savepoint;
            bool m_status;
        };

        template <typename... Elems>
        class Any : public ParserElement {
            static_assert( (IsValidElement<Elems>::value && ...) );

        public:
            static bool parse(Context& context) {
                return (AttemptParse<Elems>(context) || ...);
            }
        };

        template <typename... Elems>
        class All : public ParserElement {
            static_assert( (IsValidElement<Elems>::value && ...) );

        public:
            static bool parse(Context& context) {
                SavepointGuard savepoint{ context };
                bool parseStatus = (Elems::parse(context) && ...);

                savepoint.setStatus(parseStatus);

                return parseStatus;
            }
        };

        template <typename Elem, int LowerBound = 0, int UpperBound = -1>
        class Multiple : public ParserElement {
            static_assert(IsValidElement<Elem>::value);

            static_assert(LowerBound >= 0);
            static_assert(UpperBound <= 0 || LowerBound <= UpperBound);

        public:
            static bool parse(Context& context) {
                int instanceCount = CountInstances<Elem>(context);

                return instanceCount >= LowerBound ||
                  (UpperBound >= 0) ?
                    instanceCount <= UpperBound :
                    false;
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

    private:
        template <typename Elem>
        static bool AttemptParse(Context& context) {
            SavepointGuard savepoint{ context };
            bool parseStatus = Elem::parse(context);

            savepoint.setStatus(parseStatus);

            return parseStatus;
        }

        template <typename Elem>
        static int CountInstances(Context& context) {
            int instanceCount = 0;

            while (AttemptParse<Elem>(context)) {
                ++instanceCount;
            }
            return instanceCount;
        }
    };
}
