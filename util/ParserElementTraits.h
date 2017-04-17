#include <type_traits>

#pragma once

namespace Util {
    template <typename Elem>
    struct ParserElementTraits {
        using Context = typename Elem::ContextType;
        using Parent = typename Elem::ParentType;

        template <typename TargetContext>
        static constexpr bool IsCompatible = std::is_same<
          decltype(Elem::parse(std::declval<TargetContext&>())),
          bool
        >::value;

        static constexpr bool IsValid = IsCompatible<Context>;
    };
}
