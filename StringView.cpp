#include "StringView.h"

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
std::ostream& operator << (std::ostream& outStream, const StringView& strView) {
    outStream.write(strView.m_strPtr, strView.size());

    return outStream;
}
