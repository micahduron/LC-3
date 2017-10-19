#include <util/StringView.h>
#include "Tokenizer.h"
#include "MessageLog.h"
#include "SyntaxTree.h"

#pragma once

namespace LC3::Language {

struct ParserContext {
    Tokenizer tokenizer;
    MessageLog log;
    SyntaxTreeBuilder tree;

    ParserContext(const Util::StringView& src) :
      tokenizer{ src }
    {}
};

} // namespace LC3::Language
