#include <iostream>
#include <string>
#include <util/StringView.h>
#include "UnitTest.h"

using Util::StringView;

int main() {
    UnitTest(EmptyStringZeroSize, t) {
        StringView str;

        t.succeedIf(str.size() == 0);
    };

    UnitTest(EmptyStringNullFront, t) {
        StringView str;

        t.succeedIf(str.front() == '\0');
    };

    UnitTest(EmptyStringNullBack, t) {
        StringView str;

        t.succeedIf(str.back() == '\0');
    };

    UnitTest(StrEquality, t) {
        StringView str1 = "abcd"_sv;
        StringView str2 = "abcd"_sv;
        StringView str3 = "wxyz"_sv;

        t.succeedIf(str1 == str2 && str1 != str3);
    };

    UnitTest(StdStringConversion, t) {
        std::string stdStr = "abcd";
        StringView strView = stdStr;

        t.succeedIf(
            strView.data() == stdStr.data() &&
            strView.size() == stdStr.size()
        );
    };

    UnitTest(StrLitConversion, t) {
        const char* strLit = "abcd";
        StringView strView = strLit;

        size_t strLen = std::strlen(strLit);

        t.succeedIf(
            strView.data() == strLit &&
            strView.size() == strLen
        );
    };

    UnitTest(SubStrMatch, t) {
        StringView bigStr = "aaaxyzaaa"_sv;

        t.succeedIf(bigStr.subString(3, 3) == "xyz");
    };

    UnitTest(EndIterNull, t) {
        StringView bigStr = "abcdef"_sv;
        StringView littleStr = bigStr.subString(0, 3);

        t.succeedIf(*(littleStr.end()) == '\0');
    };

    UnitTest(StrCmpZero, t) {
        StringView str1 = "abc"_sv;
        StringView str2 = "abc"_sv;

        t.succeedIf(str1.compare(str2) == 0);
    };

    UnitTest(StrCmpLess, t) {
        StringView str1 = "abc"_sv;
        StringView str2 = "abz"_sv;

        t.succeedIf(str1.compare(str2) < 0);
    };

    UnitTest(StrCmpGreater, t) {
        StringView str1 = "xyz"_sv;
        StringView str2 = "abc"_sv;

        t.succeedIf(str1.compare(str2) > 0);
    };

    UnitTest(BeginsWith, t) {
        StringView str = "Hello, world!";

        t.succeedIf(str.beginsWith("Hello"));
    };

    UnitTest(BeginsWithTooLarge, t) {
        StringView str = "abc"_sv;

        t.failIf(str.beginsWith("abcdef"));
    };

    UnitTest(EndsWith, t) {
        StringView str = "The end."_sv;

        t.succeedIf(str.endsWith("."));
    };

    UnitTest(EndsWithTooLarge, t) {
        StringView str = "xyz"_sv;

        t.failIf(str.endsWith("xyzabc"));
    };

    UnitTest(StrHash, t) {
        StringView str1 = "abcd"_sv;
        StringView str2 = "xabcd"_sv;

        size_t hash1 = StringView::hash(str1);
        size_t hash2 = StringView::hash(str2.subString(1));
        size_t hash3 = StringView::hash(str2);

        // It is theoretically possible for hash1 == hash3, but for the
        // sake of testing it is assumed to be extremely unlikely.
        t.succeedIf(hash1 == hash2 && hash1 != hash3);
    };
    return RunTests();
}
