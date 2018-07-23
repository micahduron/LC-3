#pragma once

#include "SyntaxTreeNode.h"

namespace LC3::Language {

class TreeAnalyzer {
public:
    static bool analyze(SyntaxTreeNode& root);
};

} // namespace LC3::Language
