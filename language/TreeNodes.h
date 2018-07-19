#pragma once

#include <utility>
#include <cassert>
#include <memory>
#include <string>
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

struct NumberNode :
    public NodeBase<NodeType::Number, LC3::Word>
{
    using NodeBase::NodeBase;
};

struct StringNode :
    public NodeBase<NodeType::String, std::shared_ptr<std::string>>
{
    using NodeBase::NodeBase;
};

} // namespace LC3::Language
