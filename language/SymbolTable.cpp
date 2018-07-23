#include <cassert>
#include <utility>
#include <stack>
#include <unordered_set>
#include <Log.h>
#include <lc3.h>
#include "keywords/Directives.h"
#include "NodeType.h"
#include "TreeNodes.h"
#include "SymbolTable.h"

namespace LC3::Language {

using Keywords::Directive;

static bool LookupNames(const SyntaxTreeNode& root);
static std::optional<SymbolTable> PopulateSymbolTable(const SyntaxTreeNode& root);

std::optional<SymbolTable> SymbolTable::make(const SyntaxTreeNode& root) {
    assert(root.type == NodeType::Root);

    if (!LookupNames(root)) {
        return {};
    }
    return PopulateSymbolTable(root);
}

bool LookupNames(const SyntaxTreeNode& root) {
    bool retStatus = true;
    std::unordered_set<StringView> definedSyms;
    std::vector<const Token*> referencedSyms;

    root.walk([&definedSyms, &referencedSyms, &retStatus](const SyntaxTreeNode& node) {
        if (node.type == NodeType::LabelDefn) {
            StringView symbolName = node.token.str;

            if (definedSyms.find(symbolName) != definedSyms.end()) {
                Log::error() << node.location() << " " << node.location().getLine() << "\n"
                             << "Symbol has multiple definitions.\n";
                retStatus = false;
            } else {
                definedSyms.insert(symbolName);
            }
        } else if (node.type == NodeType::LabelRef) {
            referencedSyms.push_back(&node.token);
        }
    });
    for (const Token* refToken : referencedSyms) {
        StringView symbolName = refToken->str;

        if (definedSyms.find(symbolName) == definedSyms.end()) {
            auto& tokenLoc = refToken->location;

            Log::error() << tokenLoc << " " << tokenLoc.getLine() << "\n"
                         << "Reference to undefined symbol.\n";
            retStatus = false;
        }
    }
    return retStatus;
}

static void PopulateSymbols(std::stack<StringView>& syms, LC3::Word addr, SymbolTable& symTable) {
    for (; !syms.empty(); syms.pop()) {
        StringView symName = syms.top();

        symTable.add(symName, addr);
    }
}

static LC3::Word CalcAddress(const SyntaxTreeNode& node, LC3::Word prevAddress) {
    switch (node.type) {
        case NodeType::Instruction:
            return prevAddress + InstructionNode::size(node);
        case NodeType::Directive: {
            Directive dirType = node.data<DirectiveNode>();

            if (dirType == Directive::ORIG) {
                auto& addrVal = node.child(0).data<NumberNode>();

                return addrVal;
            } else {
                return prevAddress + DirectiveNode::size(node);
            }
        }
        default:
            break;
    }
    return prevAddress;
}

std::optional<SymbolTable> PopulateSymbolTable(const SyntaxTreeNode& root) {
    bool retStatus = true;
    SymbolTable symTable;
    LC3::Word currAddress = 0;
    std::stack<StringView> unresolvedSyms;

    for (const SyntaxTreeNode& childNode : root.children) {
        currAddress = CalcAddress(childNode, currAddress);

        switch (childNode.type) {
            case NodeType::LabelDefn:
                unresolvedSyms.push(childNode.token.str);
                break;
            case NodeType::Instruction:
                PopulateSymbols(unresolvedSyms, currAddress, symTable);
                break;
            case NodeType::Directive: {
                Directive dirType = childNode.data<DirectiveNode>();

                if (dirType == Directive::END && !unresolvedSyms.empty()) {
                    Log::error() << childNode.location() << " " << "Label to unaddressed memory.\n";
                    retStatus = false;

                    break;
                }
                PopulateSymbols(unresolvedSyms, currAddress, symTable);
                break;
            }
            default:
                break;
        }
    }
    // Check for address overflow.
    if (!retStatus) {
        return {};
    }
    return { std::move(symTable) };
}

} // namespace LC3::Language
