#pragma once

#include <lc3/Word.h>
#include "language/SyntaxTreeNode.h"

namespace LC3::Language {

class ProgramCounter {
public:
    void update(const SyntaxTreeNode& treeNode);

    LC3::Word address() const {
        return m_address;
    }

    LC3::Word nextAddress() const {
        return m_nextAddr;
    }

private:
    LC3::Word m_address = 0;
    LC3::Word m_nextAddr = 0;
};

}
