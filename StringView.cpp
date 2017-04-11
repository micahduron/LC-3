#include <cstring>
#include <utility>
#include "StringView.h"

static int LexicographicOrder(char c1, char c2);

int StringView::compare(const StringView& other, compare_func cmpFn) const {
    size_t numIters = std::min(size(), other.size());

    iterator iterOne = begin();
    iterator iterTwo = other.begin();

    while (numIters--) {
        int cmpResult = cmpFn(*iterOne++, *iterTwo++);

        if (cmpResult != 0) {
            return cmpResult;
        }
    }
    return cmpFn(*iterOne, *iterTwo);
}
int StringView::compare(const std::string& other, compare_func cmpFn) const {
    return compare(StringView(other, other.size()), cmpFn);
}
int StringView::compare(const char* other, compare_func cmpFn) const {
    auto strSize = std::strlen(other);

    return compare(StringView(other, strSize), cmpFn);
}

int StringView::compare(const StringView& other) const {
    return compare(other, LexicographicOrder);
}
int StringView::compare(const std::string& other) const {
    return compare(other, LexicographicOrder);
}
int StringView::compare(const char* other) const {
    return compare(other, LexicographicOrder);
}

bool StringView::operator == (const StringView& other) const {
    return compare(other) == 0;
}
bool StringView::operator == (const std::string& other) const {
    return compare(other) == 0;
}
bool StringView::operator == (const char* other) const {
    return compare(other) == 0;
}

std::ostream& operator << (std::ostream& outStream, const StringView& strView) {
    outStream.write(strView.m_strPtr, strView.size());

    return outStream;
}

int LexicographicOrder(char c1, char c2) {
    return c1 - c2;
}
