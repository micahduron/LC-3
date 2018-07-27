#pragma once

#include <LC3Writer.h>
#include "SymbolTable.h"
#include "SyntaxTreeNode.h"

namespace LC3::Language {

class Encoder {
public:
    static bool encode(const SyntaxTreeNode& root, const SymbolTable& symTable,
                       LC3Writer& writer);
};

} // namespace LC3::Language
