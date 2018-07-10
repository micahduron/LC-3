#pragma once

#include <unordered_map>
#include <util/StringView.h>
#include <util/StringUtils.h>

namespace Util {

template <typename ValType>
using KeywordMap = std::unordered_map<
    StringView,
    ValType,
    Str::CaselessHash,
    Str::CaselessEqual
>;

} // namespace Util
