AUTOMAKE_OPTIONS = foreign subdir-objects

TESTS = CharClass_test \
        LC3Writer_test \
        StringTokenizer_test \
        StringView_test \
        Tokenizer_test

noinst_PROGRAMS = $(TESTS)

CharClass_test_SOURCES = \
  CharClass_test.cpp \
  ../util/CharClass.cpp ../util/CharClass.h
LC3Writer_test_SOURCES = \
  LC3Writer_test.cpp
StringTokenizer_test_SOURCES = \
  StringTokenizer_test.cpp \
  ../util/StringTokenizer.h \
  ../util/StringView.h \
  ../util/CharClass.cpp ../util/CharClass.h
StringView_test_SOURCES = \
  StringView_test.cpp \
  ../util/StringView.h
Tokenizer_test_SOURCES = \
  Tokenizer_test.cpp \
  ../util/StringTokenizer.h \
  ../util/StringView.h \
  ../language/Tokenizer.cpp ../language/Tokenizer.h \
  ../util/CharClass.cpp ../util/CharClass.h
