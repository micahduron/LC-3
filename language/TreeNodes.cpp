#include "TreeNodes.h"

namespace LC3::Language {

using Keywords::Instruction;
using Keywords::Directive;

size_t InstructionNode::size(const SyntaxTreeNode& node) {
    assert(node.type == NodeType::Instruction);
    (void) node;

    return 1;
}

size_t DirectiveNode::size(const SyntaxTreeNode& node) {
    assert(node.type == NodeType::Directive);

    const auto& dirType = node.data<DirectiveNode>();

    switch (dirType) {
        case Directive::BLKW: {
            const auto& sizeField = node.child(0);
            size_t sizeVal = sizeField.data<NumberNode>();

            return sizeVal;
        }
        case Directive::STRINGZ: {
            return StringNode::size(node.child(0));
        }
        case Directive::FILL: {
            return 1;
        }
        default:
            break;
    }
    return 0;
}

size_t StringNode::size(const SyntaxTreeNode& node) {
    assert(node.type == NodeType::String);

    const auto& strVal = node.data<StringNode>();

    return strVal->size() + 1;
}

} // namespace LC3::Language
