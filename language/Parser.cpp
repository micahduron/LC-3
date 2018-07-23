#include <utility>
#include <util/ParseState.h>
#include "ParserContext.h"
#include "Grammar.h"
#include "Parser.h"

namespace LC3::Language {

using Util::ParseState;

std::optional<SyntaxTreeNode> Parser::parse(StringView src) {
    ParserContext context{ src };
    ParseState status = Grammar::Document::parse(context);

    if (status != ParseState::Success) {
        return {};
    }
    return { std::move(context.tree.treeTop()) };
}

} // namespace LC3::Language
