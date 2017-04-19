AUTOMAKE_OPTIONS = foreign subdir-objects

TESTS = CharClass_test \
        LC3Writer_test \
        StringTokenizer_test \
        StringView_test

noinst_PROGRAMS = $(TESTS)

CharClass_test_SOURCES = \
  CharClass_test.cpp \
  ../CharClass.cpp ../CharClass.h
LC3Writer_test_SOURCES = \
  LC3Writer_test.cpp \
  ../LC3Writer.cpp ../LC3Writer.h
StringTokenizer_test_SOURCES = \
  StringTokenizer_test.cpp \
  ../StringTokenizer.cpp ../StringTokenizer.h \
  ../StringView.cpp ../StringView.h \
  ../CharClass.cpp ../CharClass.h
StringView_test_SOURCES = \
  StringView_test.cpp \
  ../StringView.cpp ../StringView.h
