#pragma once

#include <deque>
#include <cstdint>
#include "SyntaxTreeNode.h"

namespace LC3::Language {

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

    template <typename NodeT, typename... ArgsT>
    SyntaxTreeNode& descendTree(ArgsT&&... args) {
        auto& rootChildren = currRoot().children;

        NodeT treeNode{ std::forward<ArgsT>(args)... };
        rootChildren.emplace_back(std::move(treeNode));

        SyntaxTreeNode* nextRoot = &rootChildren.back();
        m_nodeStack.push_back(nextRoot);

        return currRoot();
    }

    template <typename... ArgsT>
    SyntaxTreeNode& descendTree(ArgsT&&... args) {
        return descendTree<SyntaxTreeNode>(std::forward<ArgsT>(args)...);
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

} // namespace LC3::Language

