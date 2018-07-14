#pragma once

#include <vector>
#include "Token.h"
#include "NodeType.h"

namespace LC3::Language {

class SyntaxTreeBuilder;

struct SyntaxTreeNode {
    NodeType type = NodeType::Blank;
    Token token;
    std::vector<SyntaxTreeNode> children;

    SyntaxTreeNode() {}
    SyntaxTreeNode(NodeType nodeType) :
      type{ nodeType }
    {}
    SyntaxTreeNode(NodeType nodeType, const Token& nodeToken) :
      type{ nodeType },
      token{ nodeToken }
    {}

    template <typename VisitorT>
    void walk(VisitorT&& visitor) const {
        visitor(*this);

        for (const auto& child : this->children) {
            visitor(child);
        }
    }

    bool operator == (const SyntaxTreeNode& other) const {
        return this == &other;
    }
    bool operator != (const SyntaxTreeNode& other) const {
        return this != &other;
    }
};

} // namespace LC3::Language
