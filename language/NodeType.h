#pragma once

#include <iostream>

namespace LC3::Language {

enum class NodeType {
    Instruction,
    Directive,
    BranchFlags,
    LabelDefn,
    LabelRef,
    Register,
    Number,
    String,
    Blank,
    Root
};

inline std::ostream& operator << (std::ostream& outStream, NodeType nodeType) {
    switch (nodeType) {
    #define CASE(type) \
        case NodeType::type: \
            outStream << #type; \
            break

        CASE(Instruction);
        CASE(Directive);
        CASE(BranchFlags);
        CASE(LabelDefn);
        CASE(LabelRef);
        CASE(Register);
        CASE(Number);
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
