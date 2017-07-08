#include <cstring>
#include <cassert>
#include <utility>
#include "StringView.h"

namespace Util {

static int LexicographicOrder(char c1, char c2);

StringView::StringView(iterator startIter, iterator endIter) :
  StringView(startIter.m_currPtr, endIter - startIter)
{
    assert(startIter <= endIter);
}

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

StringView::iterator StringView::begin() const {
    return { data(), data() + size() };
}
StringView::iterator StringView::end() const {
    auto endPtr = data() + size();

    return { endPtr, endPtr };
}

StringView::iterator StringView::cbegin() const {
    return begin();
}
StringView::iterator StringView::cend() const {
    return end();
}

std::ostream& operator << (std::ostream& outStream, const StringView& strView) {
    outStream.write(strView.data(), strView.size());

    return outStream;
}

int LexicographicOrder(char c1, char c2) {
    return c1 - c2;
}

}
