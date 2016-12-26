#include <cstring>
#include "StringView.h"

static int LexicographicOrder(char c1, char c2);

int StringView::compareHelper(const char* str1, size_t str1Size, const char* str2,
                              size_t str2Size, compare_func cmpFn) {
    if (str1Size < str2Size) {
        return cmpFn('\0', str2[str2Size - 1]);
    } else if (str1Size > str2Size) {
        return cmpFn(str1[str1Size - 1], '\0');
    }
    auto numIters = str1Size;

    while (--numIters) {
        auto cmpResult = cmpFn(*str1++, *str2++);

        if (cmpResult != 0) {
            return cmpResult;
        }
    }
    return 0;
}

int StringView::compare(const StringView& other, compare_func cmpFn) const {
    return StringView::compareHelper(data(), size(), other.data(), other.size(), cmpFn);
}
int StringView::compare(const std::string& other, compare_func cmpFn) const {
    return StringView::compareHelper(data(), size(), other.data(), other.size(), cmpFn);
}
int StringView::compare(const char* other, compare_func cmpFn) const {
    auto strSize = std::strlen(other);

    return StringView::compareHelper(data(), size(), other, strSize, cmpFn);
}

bool StringView::operator == (const StringView& other) const {
    return compare(other, LexicographicOrder) == 0;
}
bool StringView::operator == (const std::string& other) const {
    return compare(other, LexicographicOrder) == 0;
}
bool StringView::operator == (const char* other) const {
    return compare(other, LexicographicOrder) == 0;
}

std::ostream& operator << (std::ostream& outStream, const StringView& strView) {
    outStream.write(strView.m_strPtr, strView.size());

    return outStream;
}

int LexicographicOrder(char c1, char c2) {
    return c1 - c2;
}
