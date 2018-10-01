#include <iostream>
#include <fstream>
#include <string>
#include <lc3/Word.h>
#include <Log.h>
#include <LC3Writer.h>
#include <util/StringView.h>
#include <language/Parser.h>
#include <language/TreeAnalyzer.h>
#include <language/SymbolTable.h>
#include <language/Encoder.h>

using Util::StringView;

using LC3::Log;

using LC3::Language::Parser;
using LC3::Language::TreeAnalyzer;
using LC3::Language::SymbolTable;
using LC3::Language::Encoder;

int Run(int argc, char** argv);
std::string GetSourceText(std::istream& inStream);
int Assemble(const std::string& src, LC3Writer& writer);

void PrintCount(std::ostream& outStream, size_t count, const StringView& name);

int main(int argc, char** argv) {
    int ret = Run(argc, argv);

    if (Log::errorCount() > 0) {
        std::cerr << "Assembly failed with ";
        PrintCount(std::cerr, Log::errorCount(), "error");

        std::cerr << " and ";
        PrintCount(std::cerr, Log::warningCount(), "warning");
        std::cerr << ".\n";
    } else if (Log::warningCount() > 0) {
        std::cerr << "Assembly succeeded with ";
        PrintCount(std::cerr, Log::warningCount(), "warning");
        std::cerr << ".\n";
    }
    return ret;
}

void PrintCount(std::ostream& outStream, size_t count, const StringView& name) {
    outStream << count << " " << name;

    if (count != 1) {
        outStream << "s";
    }
}

int Run(int argc, char** argv) {
    if (argc < 3) {
        Log::error() << "Incorrect number of arguments.\n"
                     << "Usage: lc3asm input_file output_file\n";
        return 1;
    }
    std::string src;
    StringView inputFilename = argv[1];
    StringView outputFilename = argv[2];

    if (inputFilename != "-") {
        if (!inputFilename.endsWith(".asm")) {
            Log::error() << "Input file must end with .asm\n";

            return 1;
        }
        std::ifstream inFile(inputFilename.data());

        src = GetSourceText(inFile);
    } else {
        inputFilename = "(stdin)";

        src = GetSourceText(std::cin);
    }
    LC3Writer writer(outputFilename.data());

    return Assemble(src, writer);
}

std::string GetSourceText(std::istream& inStream) {
    std::string srcStr;
    char strBuf[4096] = {'\0'};

    do {
        inStream.read(strBuf, sizeof(strBuf) - 1);
        auto charsRead = inStream.gcount();

        strBuf[charsRead] = '\0';
        srcStr += strBuf;
    } while (!inStream.eof());

    return srcStr;
}

int Assemble(const std::string& src, LC3Writer& writer) {
    auto asTree = Parser::parse(src);

    if (!asTree) {
        return 1;
    }
    bool analysisStatus = TreeAnalyzer::analyze(*asTree);

    if (!analysisStatus) {
        return 1;
    }
    auto symTable = SymbolTable::make(*asTree);

    if (!symTable) {
        return 1;
    }
    bool encoderStatus = Encoder::encode(*asTree, *symTable, writer);

    if (!encoderStatus) {
        return 1;
    }
    return 0;
}
