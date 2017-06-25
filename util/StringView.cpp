#include <cstring>
#include <utility>
#include "StringView.h"

namespace Util {

static int LexicographicOrder(char c1, char c2);

int StringView::compare(StringView other, compare_func cmpFn) const {
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

int StringView::compare(StringView other) const {
    return compare(other, LexicographicOrder);
}

bool StringView::operator == (StringView other) const {
    return compare(other) == 0;
}

StringView::iterator StringView::begin() const {
    return { m_strPtr, m_strPtr + m_length };
}
StringView::iterator StringView::end() const {
    auto endPtr = m_strPtr + m_length;

    return { endPtr, endPtr };
}

StringView::iterator StringView::cbegin() const {
    return begin();
}
StringView::iterator StringView::cend() const {
    return end();
}

std::ostream& operator << (std::ostream& outStream, const StringView& strView) {
    outStream.write(strView.m_strPtr, strView.size());

    return outStream;
}

int LexicographicOrder(char c1, char c2) {
    return c1 - c2;
}

}
