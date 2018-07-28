AUTOMAKE_OPTIONS = foreign subdir-objects

SUBDIRS = tests

bin_PROGRAMS = lc3asm

lc3asm_SOURCES = lc3asm.cpp \
                 lc3.h lc3.cpp \
                 Log.h Log.cpp \
                 LC3Writer.h LC3Writer.cpp \
                 language/Parser.h language/Parser.cpp \
                 language/ParserBase.h language/ParserBase.cpp \
                 language/Tokenizer.h language/Tokenizer.cpp \
                 util/CharClass.h util/CharClass.cpp \
                 language/keywords/Instructions.h language/keywords/Instructions.cpp \
                 language/keywords/Directives.h language/keywords/Directives.cpp \
                 language/TreeNodes.h language/TreeNodes.cpp \
                 language/TreeAnalyzer.h language/TreeAnalyzer.cpp \
                 language/SymbolTable.h language/SymbolTable.cpp \
                 language/Encoder.h language/Encoder.cpp \
                 language/ProgramCounter.h language/ProgramCounter.cpp
