#include <cassert>
#include <stdexcept>
#include <optional>
#include <lc3/Word.h>
#include <Log.h>
#include "keywords/Instructions.h"
#include "keywords/Directives.h"
#include "TreeNodes.h"
#include "ProgramCounter.h"
#include "Encoder.h"

namespace LC3::Language {

using Keywords::Directive;
using Keywords::Instruction;
using Keywords::Instructions;

static bool EncodeDirective(const SyntaxTreeNode& dirNode, const SymbolTable& symTable,
                            const ProgramCounter& progCounter, LC3Writer& writer);
static bool EncodeInstruction(const SyntaxTreeNode& instrNode, const SymbolTable& symTable,
                              const ProgramCounter& progCounter, LC3Writer& writer);

static LC3::Word GetNodeValue(const SyntaxTreeNode& treeNode, const SymbolTable& symTable);

static std::optional<LC3::Word> RestrictWidth(LC3::Word word, size_t numBits);
static std::optional<LC3::Word> RestrictWidthSigned(LC3::Word word, size_t numBits);

bool Encoder::encode(const SyntaxTreeNode& rootNode, const SymbolTable& symTable,
                     LC3Writer& writer)
{
    assert(rootNode.type == NodeType::Root);

    ProgramCounter progCounter;
    bool encodeStatus = true;

    for (const SyntaxTreeNode& childNode : rootNode.children) {
        bool status = true;

        progCounter.update(childNode);

        switch (childNode.type) {
            case NodeType::Directive:
                status = EncodeDirective(childNode, symTable, progCounter, writer);
                break;
            case NodeType::Instruction:
                status = EncodeInstruction(childNode, symTable, progCounter, writer);
                break;
            default:
                break;
        }
        if (!status) {
            encodeStatus = false;
        }
    }
    return encodeStatus;
}

bool EncodeDirective(const SyntaxTreeNode& dirNode, const SymbolTable& symTable,
                     const ProgramCounter& progCounter, LC3Writer& writer)
{
    assert(dirNode.type == NodeType::Directive);

    (void) progCounter;

    bool retStatus = true;
    Directive dirType = dirNode.data<DirectiveNode>();

    switch (dirType) {
        case Directive::ORIG: {
            LC3::Word startAddr = dirNode.child(0).data<NumberNode>();

            writer.putWord(startAddr);
            break;
        }
        case Directive::FILL: {
            auto fillContents = GetNodeValue(dirNode.child(0), symTable);

            writer.putWord(fillContents);
            break;
        }
        case Directive::BLKW: {
            LC3::Word wordCount = dirNode.child(0).data<NumberNode>();
            LC3::Word memVal = dirNode.children.size() > 1 ?
                                GetNodeValue(dirNode.child(1), symTable) :
                                0;
            for (LC3::WordValue i = 0; i < wordCount.value(); ++i) {
                writer.putWord(memVal);
            }
            break;
        }
        case Directive::STRINGZ: {
            const std::string& strVal = *dirNode.child(0).data<StringNode>();

            for (char c : strVal) {
                writer.putWord(c);
            }
            writer.putWord('\0');
            break;
        }
        case Directive::END:
        case Directive::Invalid:
            break;
    }
    return retStatus;
}

static LC3::Word GetRawOffset(const SyntaxTreeNode& addrNode,
                               const SymbolTable& symTable, const ProgramCounter& progCounter) {
    auto addrVal = GetNodeValue(addrNode, symTable);

    return LC3::Word(addrVal.value() - progCounter.nextAddress().value());
}

static std::optional<LC3::Word> GetOffset(const SyntaxTreeNode& addrNode, const SymbolTable& symTable,
                            const ProgramCounter& progCounter, size_t bitWidth)
{
    auto rawOffset = GetRawOffset(addrNode, symTable, progCounter);
    auto offsetVal = RestrictWidthSigned(rawOffset, bitWidth);

    if (!offsetVal) {
        Log::error(addrNode) << "Offset cannot fit within " << bitWidth << " bits (" << rawOffset << ").\n";

        return {};
    }
    return offsetVal;
}

static LC3::Word GetBranchFlags(const SyntaxTreeNode& flagsNode) {
    assert(flagsNode.type == NodeType::BranchFlags);

    LC3::WordValue flagsVal = 0;
    const auto& branchFlags = flagsNode.data<BRFlagsNode>();

    flagsVal |= branchFlags.n ? (1 << 2) : 0;
    flagsVal |= branchFlags.z ? (1 << 1) : 0;
    flagsVal |= branchFlags.p ? (1 << 0) : 0;

    return LC3::Word(flagsVal);
}

static std::optional<LC3::Word> GetEncodedInstruction(const SyntaxTreeNode& instrNode,
                                                      const SymbolTable& symTable,
                                                      const ProgramCounter& progCounter)
{
    assert(instrNode.type == NodeType::Instruction);

    const auto& instrData = instrNode.data<InstructionNode>();
    LC3::Word opcode = Instructions::getOpcode(instrData.type);

    switch (instrData.format) {
        case NodeFormat::Empty:
            return { opcode };
        case NodeFormat::Vec: {
            const auto& vecChild = instrNode.child(0);
            auto rawVec = GetNodeValue(vecChild, symTable);
            auto vecVal = RestrictWidth(rawVec, 8);

            if (!vecVal) {
                Log::error(vecChild) << "Vector is larger than 8 bits "
                                        << "(" << rawVec << ").\n";
                return { std::nullopt };
            }
            return { opcode | *vecVal };
        }
        case NodeFormat::Addr: {
            auto offsetVal = GetOffset(instrNode.child(0), symTable, progCounter, 11);

            if (!offsetVal) {
                return { std::nullopt };
            }
            return { opcode | *offsetVal };
        }
        case NodeFormat::Branch: {
            auto flagsVal = GetBranchFlags(instrNode.child(0));
            auto offsetVal = GetOffset(instrNode.child(1), symTable, progCounter, 9);

            if (!offsetVal) {
                return { std::nullopt };
            }
            return { opcode | (flagsVal << 9) | *offsetVal };
        }
        case NodeFormat::RegReg: {
            auto regOne = instrNode.child(0).data<RegisterNode>();
            auto regTwo = instrNode.child(1).data<RegisterNode>();

            return { opcode | (regOne << 9) | (regTwo << 6) };
        }
        case NodeFormat::RegAddr: {
            auto regOne = instrNode.child(0).data<RegisterNode>();
            auto& offsetChild = instrNode.child(1);
            auto offsetVal = GetOffset(offsetChild, symTable, progCounter, 9);

            if (!offsetVal) {
                Log::error(offsetChild) << "Offset cannot fit within 9 bits.\n";

                return { std::nullopt };
            }
            return { opcode | (regOne << 9) | *offsetVal };
        }
        case NodeFormat::RegRegReg: {
            auto regOne = instrNode.child(0).data<RegisterNode>();
            auto regTwo = instrNode.child(1).data<RegisterNode>();
            auto regThree = instrNode.child(2).data<RegisterNode>();

            return { opcode | (regOne << 9) | (regTwo << 6) | regThree };
        }
        case NodeFormat::RegRegNum: {
            auto regOne = instrNode.child(0).data<RegisterNode>();
            auto regTwo = instrNode.child(1).data<RegisterNode>();

            auto& numChild = instrNode.child(2);
            auto rawNum = GetNodeValue(numChild, symTable);
            auto numVal = RestrictWidthSigned(rawNum, 5);

            if (!numVal) {
                Log::error(numChild) << "Imediate cannot fit within 5 bits.\n";

                return { std::nullopt };
            }
            return { opcode | (regOne << 9) | (regTwo << 6) | (1 << 5) | *numVal };
        }
        case NodeFormat::RegRegAddr: {
            auto regOne = instrNode.child(0).data<RegisterNode>();
            auto regTwo = instrNode.child(1).data<RegisterNode>();

            auto& offsetChild = instrNode.child(2);
            auto rawOffset = GetNodeValue(offsetChild, symTable);
            auto offsetVal = RestrictWidthSigned(rawOffset, 6);

            if (!offsetVal) {
                Log::error(offsetChild) << "Offset cannot fit within 6 bits.\n";
                return { std::nullopt };
            }
            return { opcode | (regOne << 9) | (regTwo << 6) | *offsetVal };
        }
        default:
            Log::error(instrNode) << "Unimplemented instruction format " << instrData.format << "\n";

            throw std::logic_error("Oops.");
    }
    return { std::nullopt };
}

bool EncodeInstruction(const SyntaxTreeNode& instrNode, const SymbolTable& symTable,
                       const ProgramCounter& progCounter, LC3Writer& writer)
{
    assert(instrNode.type == NodeType::Instruction);

    auto encodedInstr = GetEncodedInstruction(instrNode, symTable, progCounter);
    
    if (!encodedInstr) {
        return false;
    }
    writer.putWord(*encodedInstr);

    return true;
}

LC3::Word GetNodeValue(const SyntaxTreeNode& treeNode, const SymbolTable& symTable) {
    assert(treeNode.type == NodeType::Number || treeNode.type == NodeType::LabelRef);

    if (treeNode.type == NodeType::Number) {
        return treeNode.data<NumberNode>();
    }
    StringView labelName = treeNode.token.str;
    auto labelAddr = symTable.get(labelName);

    assert(labelAddr.has_value());

    return *labelAddr;
}

static std::optional<LC3::Word> RestrictWidth_Helper(LC3::Word word, size_t numBits, bool isSigned) {
    assert(numBits <= LC3::Word::numBits);

    LC3::WordValue maxValue = (1 << (numBits + 1)) - 1;
    LC3::WordValue testValue = isSigned ? word.abs().value() : word.value();

    if (testValue > maxValue) {
        return std::nullopt;
    }
    LC3::WordValue bitMask = (1 << numBits) - 1;

    return { LC3::Word(word.value() & bitMask) };
}

std::optional<LC3::Word> RestrictWidth(LC3::Word word, size_t numBits) {
    return RestrictWidth_Helper(word, numBits, false);
}

std::optional<LC3::Word> RestrictWidthSigned(LC3::Word word, size_t numBits) {
    return RestrictWidth_Helper(word, numBits, true);
}

} // namespace LC3::Language
