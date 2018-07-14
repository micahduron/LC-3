#pragma once

#include <utility>
#include <cassert>
#include <lc3.h>
#include "SyntaxTreeNode.h"
#include "keywords/Instructions.h"
#include "keywords/Directives.h"

namespace LC3::Language {

using Keywords::Directive;

template <NodeType NodeT, typename ContainerT>
struct NodeBase : public SyntaxTreeNode {
    using DataContainer = ContainerT;

    template <typename... ArgsT>
    NodeBase(DataContainer data, ArgsT&&... args) :
      SyntaxTreeNode(NodeT, std::forward<ArgsT>(args)...)
    {
        this->m_data = std::move(data);
    }
};

struct InstructionNode :
    public NodeBase<NodeType::Instruction, Keywords::Instruction>
{
    using NodeBase::NodeBase;
};

struct DirectiveNode :
    public NodeBase<NodeType::Directive, Keywords::Directive>
{
    using NodeBase::NodeBase;
};

struct RegisterNode :
    public NodeBase<NodeType::Register, int>
{
    using NodeBase::NodeBase;
};

} // namespace LC3::Language
