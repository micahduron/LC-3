#include <iostream>
#include <functional>
#include <cstring>
#include <cctype>
#include <util/CharClass.h>
#include "UnitTest.h"

using Util::CharClass;

TestFunction MakeTest(std::function<bool()> testFn);

int main() {
    CharClass isAlpha([](char c) -> bool {
        auto cu = static_cast<unsigned char>(c);

        return isalpha(cu);
    });
    CharClass isLetterJ("J");
    CharClass isNum{ [](char c)->bool { return c >= '0' && c <= '9'; } };

    auto lambdaInit = [isAlpha]() {
        return isAlpha('a');
    };
    UnitTestFn(LambdaInit, MakeTest(lambdaInit));

    auto strInit = [isLetterJ]() {
        return isLetterJ('J') && !isLetterJ('j');
    };
    UnitTestFn(TestStrInit, MakeTest(strInit));

    auto complementTest = [isNotJ = ~isLetterJ]() {
        return !isNotJ('J') && isNotJ('j');
    };
    UnitTestFn(Complement, MakeTest(complementTest));

    auto unionTest = [isAlnum = isAlpha | isNum]() {
        return isAlnum('A') && isAlnum('9');
    };
    UnitTestFn(Union, MakeTest(unionTest));

    auto intersectTest = []() {
        CharClass is123 = CharClass("abc123") & CharClass("123xyz");

        return !is123('a') && is123('2') && !is123('z');
    };
    UnitTestFn(Intersect, MakeTest(intersectTest));

    return RunTests();
}

TestFunction MakeTest(std::function<bool()> testFn) {
    return [testFn](TestContext& t) {
        t.succeedIf(testFn());
    };
}
