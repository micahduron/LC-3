#include <type_traits>
#include <utility>
#include "ParseState.h"

#pragma once

namespace Util {
    namespace Internals {
        template <typename Elem, typename Parser, typename = void>
        class IsCompatible_Helper : public std::false_type {};

        template <typename Elem, typename Parser>
        class IsCompatible_Helper<
          Elem,
          Parser,
          typename std::enable_if<
            std::is_same<
              ParseState,
              decltype(Elem::parse(std::declval<typename Parser::ContextType&>()))
            >::value,
            void
          >::type
        > : public std::true_type {};
    }

    template <typename Elem>
    struct ParserElementTraits {
        using Context = typename Elem::ContextType;
        using Parent = typename Elem::ParentType;

        template <typename TargetParser>
        static constexpr bool IsCompatible = Internals::IsCompatible_Helper<Elem, TargetParser>::value;

        static_assert(IsCompatible<Parent> == true);
    };
}
