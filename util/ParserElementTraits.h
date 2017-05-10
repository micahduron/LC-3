#include <type_traits>
#include <utility>
#include <void_t>

#pragma once

namespace Util {
    namespace Internals {
        template <typename Elem, typename Parser, typename = std::void_t<>>
        class IsCompatible_Helper : public std::false_type {};

        template <typename Elem, typename Parser>
        class IsCompatible_Helper<
          Elem,
          Parser,
          std::void_t<
            decltype(!Elem::parse(std::declval<typename Parser::ContextType&>()))
          >
        > : public std::true_type
        {};
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
