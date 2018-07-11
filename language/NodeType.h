#pragma once

#include <iostream>

namespace LC3::Language {

enum class NodeType {
    Instruction,
    Directive,
    LabelDefn,
    LabelRef,
    Register,
    HexNumber,
    DecNumber,
    NegNumber,
    String,
    Blank,
    Root
};

std::ostream& operator << (std::ostream& outStream, NodeType nodeType) {
    switch (nodeType) {
    #define CASE(type) \
        case NodeType::type: \
            outStream << #type; \
            break

        CASE(Instruction);
        CASE(Directive);
        CASE(LabelDefn);
        CASE(LabelRef);
        CASE(Register);
        CASE(HexNumber);
        CASE(DecNumber);
        CASE(NegNumber);
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
