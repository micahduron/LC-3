#include <util/CharClass.h>
#include <util/StringTokenizer.h>
#include <util/StringView.h>
#include "UnitTest.h"

using Util::CharClass;
using Util::StringTokenizer;
using Util::StringView;

TestFunction TestSkip(const StringView& src, size_t skipCount);

int main() {
    StringView skipSrc = "abcde"_sv;

    UnitTestFn(StartPos, TestSkip(skipSrc, 0));
    UnitTestFn(Skip, TestSkip(skipSrc, skipSrc.size() - 1));
    UnitTestFn(SkipOvershoot, TestSkip(skipSrc, skipSrc.size() + 100));

    UnitTest(PeekChar, t) {
        StringTokenizer tok{ "abcd"_sv };
        tok.skip(2);

        t.succeedIf(tok.peekChar() == 'c');
    };

    UnitTest(PeekStr, t) {
        StringTokenizer tok{ "abcxyz"_sv };

        t.succeedIf(tok.peekStr(3, 3) == "xyz");
    };

    UnitTest(PeekStrOvershoot, t) {
        StringTokenizer tok{ "abcxyz"_sv };

        t.succeedIf(tok.peekStr(50, 3) == "xyz");
    };

    UnitTest(PeekCharOvershoot, t) {
        StringView src = "abcd"_sv;
        StringTokenizer tok{ src };
        tok.skip(src.size());

        t.succeedIf(tok.peekChar() == '\0');
    };

    UnitTest(SkipUntil, t) {
        StringTokenizer tok{ "aaab"_sv };
        tok.skipUntil(CharClass("b"));

        t.succeedIf(tok.peekChar() == 'b');
    };

    UnitTest(SkipUntilNot, t) {
        StringTokenizer tok{ "aaab" };
        tok.skipUntilNot(CharClass("a"));

        t.succeedIf(tok.peekChar() == 'b');
    };

    UnitTest(Read, t) {
        StringView src = "abcd"_sv;
        StringTokenizer tok{ src };

        t.succeedIf(tok.read(src.size() - 1) == src.subString(0, src.size() - 1));
    };

    UnitTest(ReadUntil, t) {
        StringTokenizer tok{ "abcd wxyz"_sv };

        StringView readResult = tok.readUntil(CharClass(" "));

        t.succeedIf(readResult == "abcd");
    };

    UnitTest(ReadUntilNot, t) {
        StringTokenizer tok{ "aaabccc"_sv };

        StringView readResult = tok.readUntilNot(CharClass("a"));

        t.succeedIf(readResult == "aaa");
    };

    UnitTest(NextToken, t) {
        StringView src = " a + b + c "_sv;
        StringTokenizer tok{ src };

        CharClass separators(" +");

        t.succeedIf(
            tok.nextToken(separators) == "a" &&
            tok.nextToken(separators) == "b" &&
            tok.nextToken(separators) == "c"
        );
    };
    return RunTests();
}

TestFunction TestSkip(const StringView& src, size_t skipCount) {
    return [&src, skipCount](TestContext& t) -> void {
        StringTokenizer tok{ src };
        tok.skip(skipCount);

        size_t pos = std::min(src.size(), skipCount);
        size_t rem = src.size() - pos;

        t.succeedIf(tok.position() == pos && tok.remaining() == rem);
    };
}
