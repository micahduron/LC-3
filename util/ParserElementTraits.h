#include <type_traits>

#pragma once

namespace Util {
    template <typename Elem>
    struct ParserElementTraits {
        using Context = typename Elem::ContextType;
        using Parent = typename Elem::ParentType;

        template <typename TargetParser>
        static constexpr bool IsCompatible = std::is_same<
          decltype(Elem::parse(std::declval<typename TargetParser::ContextType&>())),
          bool
        >::value;

        static_assert(IsCompatible<Parent> == true);
    };
}
