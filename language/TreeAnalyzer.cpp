#include <Log.h>
#include <util/StringView.h>
#include <util/GenericParser.h>
#include <util/ParseState.h>
#include "TreeNodes.h"
#include "keywords/Instructions.h"
#include "keywords/Directives.h"
#include "NodeType.h"
#include "TreeAnalyzer.h"

namespace LC3::Language {

using Util::StringView;
using Util::ParseState;
using Keywords::Instruction;
using Keywords::Directive;

struct AnalyzerFlags {
    bool addressedMemory = false;
};

static bool AnalyzeTreeNode(SyntaxTreeNode& node, AnalyzerFlags& flags);
static bool AnalyzeInstruction(SyntaxTreeNode& node, AnalyzerFlags& flags);
static bool AnalyzeDirective(SyntaxTreeNode& node, AnalyzerFlags& flags);

struct CheckerContext {
    const SyntaxTreeNode* node = nullptr;
    size_t childIndex = 0;
    NodeFormat format = NodeFormat::Invalid;

    CheckerContext& operator () (const SyntaxTreeNode& treeNode) {
        node = &treeNode;
        childIndex = 0;

        return *this;
    }

    const SyntaxTreeNode& parent() const {
        return *node;
    }

    const SyntaxTreeNode& currentChild() const {
        return node->child(childIndex);
    }
};

using NodeChecker = Util::GenericParser<CheckerContext>;

template <typename Ts_Head, typename... Ts_Tail>
struct TypeList {
    using head = Ts_Head;
    using tail = TypeList<Ts_Tail...>;

    static constexpr size_t remaining = 1 + tail::remaining;
};

template <typename T>
struct TypeList<T> {
    using head = T;

    static constexpr size_t remaining = 0;
};

template <NodeFormat FormatV, typename... CheckerTs>
struct FormatSpec {
    using checker = NodeChecker::All<CheckerTs...>;
    using elements = TypeList<CheckerTs...>;
    static constexpr NodeFormat formatVal = FormatV;
};

template <NodeType NodeT>
struct BasicCheck : public NodeChecker::ParserElement {
    static ParseState parse(CheckerContext& ctx) {
        if (ctx.childIndex >= ctx.parent().children.size()) {
            return ParseState::NonFatalFail;
        }
        const auto& currChild = ctx.currentChild();

        if (currChild.type == NodeT) {
            ++ctx.childIndex;

            return ParseState::Success;
        }
        return ParseState::NonFatalFail;
    }

    static void print(std::ostream& outStream) {
        outStream << NodeT;
    }
};

using Num = BasicCheck<NodeType::Number>;
using Reg = BasicCheck<NodeType::Register>;
using Label = BasicCheck<NodeType::LabelRef>;
using Str = BasicCheck<NodeType::String>;

struct Addr : public NodeChecker::ParserElement {
    static ParseState parse(CheckerContext& ctx) {
        return NodeChecker::Any<Num, Label>::parse(ctx);
    }

    static void print(std::ostream& outStream) {
        outStream << "Address";
    }
};

struct EmptyNode : public NodeChecker::ParserElement {
    static ParseState parse(CheckerContext& ctx) {
        return ctx.parent().children.size() == 0 ?
            ParseState::Success :
            ParseState::NonFatalFail;
    }

    static void print(std::ostream& outStream) {
        outStream << "Empty";
    }
};

using Empty = FormatSpec<NodeFormat::Empty, EmptyNode>;

using Register = FormatSpec<NodeFormat::Reg, Reg>;
using Number = FormatSpec<NodeFormat::Num, Num>;
using Address = FormatSpec<NodeFormat::Addr, Addr>;
using String = FormatSpec<NodeFormat::Str, Str>;
using Vector = FormatSpec<NodeFormat::Vec, Num>;

using NumNum = FormatSpec<NodeFormat::NumNum, Num, Num>;
using NumAddr = FormatSpec<NodeFormat::NumAddr, Num, Addr>;
using RegReg = FormatSpec<NodeFormat::RegReg, Reg, Reg>;
using RegNum = FormatSpec<NodeFormat::RegNum, Reg, Num>;
using RegAddr = FormatSpec<NodeFormat::RegAddr, Reg, Addr>;

using RegRegNum = FormatSpec<NodeFormat::RegRegNum, Reg, Reg, Num>;
using RegRegAddr = FormatSpec<NodeFormat::RegRegAddr, Reg, Reg, Addr>;
using RegRegReg = FormatSpec<NodeFormat::RegRegReg, Reg, Reg, Reg>;

template <typename... SpecTs>
static NodeFormat CheckNode(const SyntaxTreeNode& node);

bool TreeAnalyzer::analyze(SyntaxTreeNode& root) {
    bool status = true;
    AnalyzerFlags flags;

    root.walk([&status, &flags](SyntaxTreeNode& node) {
        if (!AnalyzeTreeNode(node, flags)) {
            status = false;
        }
    });
    if (flags.addressedMemory) {
        Log::warning() << "Unmatched .ORIG directive. Did you forget "
                       << "to put .END at the end of the file?\n";
    }
    return status;
}

bool AnalyzeTreeNode(SyntaxTreeNode& node, AnalyzerFlags& flags) {
    switch (node.type) {
        case NodeType::Instruction:
            return AnalyzeInstruction(node, flags);
        case NodeType::Directive:
            return AnalyzeDirective(node, flags);
        default:
            break;
    }
    return true;
}

static NodeFormat CheckBranch(const SyntaxTreeNode& node) {
    StringView flags = node.token.str.subString(2);

    for (char flag : flags) {
        switch (std::tolower(flag)) {
            case 'n':
            case 'z':
            case 'p':
                break;
            default:
                Log::error() << "Unrecognized branch flag.\n";

                return NodeFormat::Invalid;
        }
    }
    return CheckNode<Address>(node);
}

static NodeFormat GetInstructionFormat(const SyntaxTreeNode& node) {
    assert(node.type == NodeType::Instruction);

    const auto& instrData = node.data<InstructionNode>();

    switch (instrData.type) {
    #define I(Ins) Instruction::Ins
        case I(ADD):
            return CheckNode<RegRegNum, RegRegReg>(node);
        case I(AND):
            return CheckNode<RegRegNum, RegRegReg>(node);
        case I(BR):
            return CheckBranch(node);
        case I(JMP):
            return CheckNode<Address>(node);
        case I(JSR):
            return CheckNode<Address>(node);
        case I(JSRR):
            return CheckNode<Register>(node);
        case I(LD):
            return CheckNode<RegAddr>(node);
        case I(LDI):
            return CheckNode<RegAddr>(node);
        case I(LDR):
            return CheckNode<RegRegAddr>(node);
        case I(LEA):
            return CheckNode<RegAddr>(node);
        case I(NOT):
            return CheckNode<RegReg>(node);
        case I(RET):
            return CheckNode<Empty>(node);
        case I(RTI):
            return CheckNode<Empty>(node);
        case I(ST):
            return CheckNode<RegAddr>(node);
        case I(STI):
            return CheckNode<RegAddr>(node);
        case I(STR):
            return CheckNode<RegRegAddr>(node);
        case I(TRAP):
            return CheckNode<Vector>(node);
        case I(GETC):
        case I(PUTS):
        case I(PUTSP):
        case I(OUT):
        case I(IN):
        case I(HALT):
            return CheckNode<Empty>(node);
        default:
            break;
    #undef I
    }
    return NodeFormat::Invalid;
}

bool AnalyzeInstruction(SyntaxTreeNode& node, AnalyzerFlags& flags) {
    if (!flags.addressedMemory) {
        Log::error() << "Instruction in unaddressed memory.\n"
                     << "Use the .ORIG and .END directives to "
                     << "designate an addressed region of memory.\n";
        return false;
    }
    NodeFormat instrFormat = GetInstructionFormat(node);

    if (instrFormat == NodeFormat::Invalid) {
        return false;
    }
    auto& instrData = node.data<InstructionNode>();
    instrData.format = instrFormat;

    return true;
}

bool AnalyzeDirective(SyntaxTreeNode& node, AnalyzerFlags& flags) {
    Directive dirType = node.data<DirectiveNode>();
    NodeFormat nodeFormat = NodeFormat::Invalid;

    if (!flags.addressedMemory && dirType != Directive::ORIG) {
        auto& err = Log::error();
        err << node.location() << node.location().getLine() << "\n";

        if (dirType != Directive::END) {
            err << "Memory allocation in unaddressed memory."
                << "Use the .ORIG and .END directives to designate "
                << "addressed memory.\n";
        } else {
            err << "Found an .END directive without a corresponding .ORIG.\n";
        }
        return false;
    }
    switch (dirType) {
    #define D(Dir) Directive::Dir
        case D(ORIG):
            if (flags.addressedMemory) {
                Log::error() << "Nested .ORIG directives is not allowed.\n";

                return false;
            }
            nodeFormat = CheckNode<Number>(node);
            flags.addressedMemory = true;
            break;
        case D(FILL):
            nodeFormat = CheckNode<Address>(node);
            break;
        case D(BLKW):
            nodeFormat = CheckNode<Number, NumNum, NumAddr>(node);
            break;
        case D(STRINGZ):
            nodeFormat = CheckNode<String>(node);
            break;
        case D(END):
            nodeFormat = CheckNode<Empty>(node);
            flags.addressedMemory = false;
            break;
        default:
            Log::error() << dirType << "\n";
    #undef D
    }
    return nodeFormat != NodeFormat::Invalid;
}

template <typename ElemListT>
static void PrintSpec_Helper(std::ostream& outStream) {
    ElemListT::head::print(outStream);

    if constexpr (ElemListT::remaining > 0) {
        outStream << ", ";

        PrintSpec_Helper<typename ElemListT::tail>(outStream);
    }
}

template <typename SpecT>
static void PrintSpec(std::ostream& outStream) {
    PrintSpec_Helper<typename SpecT::elements>(outStream);
}

template <typename SpecTs_Head, typename... SpecTs_Tail>
static void PrintSpecs(std::ostream& outStream) {
    PrintSpec<SpecTs_Head>(outStream);
    outStream << "\n";

    if constexpr (sizeof...(SpecTs_Tail) > 0) {
        PrintSpecs<SpecTs_Tail...>(outStream);
    }
}

template <typename SpecT>
static bool CheckNodeSingle(CheckerContext& ctx, const SyntaxTreeNode& node) {
    if (SpecT::checker::parse(ctx(node)) == ParseState::Success) {
        ctx.format = SpecT::formatVal;

        return true;
    }
    return false;
}

template <typename... SpecTs>
NodeFormat CheckNode(const SyntaxTreeNode& node) {
    CheckerContext ctx;

    if (!(CheckNodeSingle<SpecTs>(ctx, node) || ...)) {
        auto& err = Log::error();

       err << node.location() << " " << node.location().getLine() << "\n"
            << "No matching format found. Valid formats are:\n";
        PrintSpecs<SpecTs...>(err);
        err << '\n';
    }
    return ctx.format;
}

} // namespace LC3::Language
