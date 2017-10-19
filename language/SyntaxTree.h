#include <iostream>
#include <stdexcept>
#include <vector>
#include <deque>
#include "Token.h"

#pragma once


namespace LC3::Language {

enum class NodeType {
    Directive,
    Instruction,
    LabelDefn,
    LabelRef,
    Register,
    HexNumber,
    DecNumber,
    String,
    Blank,
    Root
};

class SyntaxTreeBuilder;

struct SyntaxTreeNode {
    NodeType type = NodeType::Blank;
    Token token;

    SyntaxTreeNode() {}
    SyntaxTreeNode(NodeType nodeType) :
      type{ nodeType }
    {}
    SyntaxTreeNode(NodeType nodeType, const Token& nodeToken) :
      type{ nodeType },
      token{ nodeToken }
    {}

    const std::vector<SyntaxTreeNode>& children() const {
        return m_children;
    }

    bool operator == (const SyntaxTreeNode& other) const {
        return this == &other;
    }
    bool operator != (const SyntaxTreeNode& other) const {
        return this != &other;
    }

private:
    std::vector<SyntaxTreeNode> m_children;

    friend class SyntaxTreeBuilder;
};

class SyntaxTreeBuilder {
private:
    using node_stack = std::deque<SyntaxTreeNode*>;

public:
    SyntaxTreeBuilder() :
      m_treeTop{ NodeType::Root },
      m_nodeStack{ &m_treeTop }
    {}

    SyntaxTreeNode& currRoot() {
        return *m_nodeStack.back();
    }
    const SyntaxTreeNode& currRoot() const {
        return *m_nodeStack.back();
    }

    SyntaxTreeNode& treeTop() {
        return m_treeTop;
    }
    const SyntaxTreeNode& treeTop() const {
        return m_treeTop;
    }

    SyntaxTreeNode& ascendTree() {
        if (m_nodeStack.size() > 1) {
            m_nodeStack.pop_back();
        }
        return currRoot();
    }

    SyntaxTreeNode& descendTree() {
        auto& rootChildren = currRoot().m_children;

        rootChildren.emplace_back();

        SyntaxTreeNode* nextRoot = &rootChildren.back();
        m_nodeStack.push_back(nextRoot);

        return currRoot();
    }

    class DescentGuard {
    public:
        DescentGuard(SyntaxTreeBuilder& builder) :
          m_stack{ builder.m_nodeStack },
          m_origStackLen{ m_stack.size() }
        {}

        ~DescentGuard() {
            auto origTop = m_stack.begin() + m_origStackLen;
            auto currTop = m_stack.end();

            m_stack.erase(origTop, currTop);
        }

    private:
        node_stack& m_stack;
        size_t m_origStackLen;
    };

private:
    SyntaxTreeNode m_treeTop;
    node_stack m_nodeStack;

    friend class DescentGuard;
};

std::ostream& operator << (std::ostream& outStream, NodeType nodeType) {
    switch (nodeType) {
    #define CASE(type) \
        case NodeType::type: \
            outStream << #type; \
            break

        CASE(Directive);
        CASE(Instruction);
        CASE(LabelDefn);
        CASE(LabelRef);
        CASE(Register);
        CASE(HexNumber);
        CASE(DecNumber);
        CASE(String);
        CASE(Blank);
        CASE(Root);

    #undef CASE

        default:
            throw std::domain_error("Unimplemented NodeType case.");
    }
    return outStream;
}

} // namespace LC3::Language
