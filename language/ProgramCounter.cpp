#include "TreeNodes.h"
#include "keywords/Directives.h"
#include "ProgramCounter.h"

namespace LC3::Language {

void ProgramCounter::update(const SyntaxTreeNode& treeNode) {
    switch (treeNode.type) {
        case NodeType::Instruction:
            m_address = m_nextAddr;
            m_nextAddr = m_address + InstructionNode::size(treeNode);

            break;
        case NodeType::Directive: {
            Directive dirType = treeNode.data<DirectiveNode>();

            if (dirType == Directive::ORIG) {
                m_address = treeNode.child(0).data<NumberNode>();
                m_nextAddr = m_address;
            } else {
                m_address = m_nextAddr;
                m_nextAddr = m_address + DirectiveNode::size(treeNode);
            }
            break;
        }
        default:
            break;
    }
}

}
