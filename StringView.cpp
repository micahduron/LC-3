#include "StringView.h"

int StringView::compareHelper(const char* str1, size_t str1Size const char* str2,
                              size_t str2Size, compare_func cmpFn) {
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
