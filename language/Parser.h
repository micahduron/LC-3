#include <optional>
#include "SyntaxTreeNode.h"
#include "Token.h"
#include "ParserContext.h"

#pragma once

namespace LC3::Language {

using Util::StringView;

class Parser {
public:
    static std::optional<SyntaxTreeNode> parse(StringView src);
};

} // namespace LC3::Language
