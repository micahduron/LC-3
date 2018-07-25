#pragma once

#include <utility>
#include <memory>
#include <string>
#include <lc3.h>
#include "SyntaxTreeNode.h"
#include "NodeFormat.h"
#include "keywords/Instructions.h"
#include "keywords/Directives.h"

namespace LC3::Language {

using Keywords::Instruction;
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

struct InstructionData {
    Instruction type = Instruction::Invalid;
    NodeFormat format = NodeFormat::Invalid;

    struct {
        bool n;
        bool z;
        bool p;
    } flags = {false, false, false};
};

struct InstructionNode :
    public NodeBase<NodeType::Instruction, InstructionData>
{
    using NodeBase::NodeBase;

    static size_t size(const SyntaxTreeNode& node);
};

struct DirectiveNode :
    public NodeBase<NodeType::Directive, Directive>
{
    using NodeBase::NodeBase;

    static size_t size(const SyntaxTreeNode& node);
};

struct RegisterNode :
    public NodeBase<NodeType::Register, LC3::Word>
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

    static size_t size(const SyntaxTreeNode& node);
};

} // namespace LC3::Language
