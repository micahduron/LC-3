#include <type_traits>
#include <utility>

#pragma once

namespace Util {
    template <typename Elem>
    struct ParserElementTraits {
        using Context = typename Elem::ContextType;
        using Parent = typename Elem::ParentType;

        template <typename TargetContext>
        static constexpr bool IsCompatible = std::conditional_t<
          std::is_same<decltype(Elem::parse(std::declval<TargetContext&>())), bool>::value,
          std::true_type,
          std::false_type
        >::value;

        static constexpr bool IsValid = IsCompatible<Context>;
    };
}
