#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <utility>
#include <type_traits>
#include <functional>
#include "Hash.h"

#pragma once

namespace Util {

class StringView;

namespace Internals {

class StringView_iterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = const char*;
    using reference = const char&;
    using iterator_category = std::random_access_iterator_tag;

    constexpr StringView_iterator(const StringView_iterator& other) = default;
    constexpr StringView_iterator(StringView_iterator&& other) = default;

    constexpr StringView_iterator(const char* currPtr, const char* endPtr) :
      m_currPtr{ currPtr },
      m_endPtr{ endPtr }
    {}

    constexpr StringView_iterator& operator = (const StringView_iterator& other) = default;
    constexpr StringView_iterator& operator = (StringView_iterator&& other) = default;

    // Pointer dereference operator
    constexpr value_type operator * () const {
        return m_currPtr != m_endPtr ? *m_currPtr : '\0';
    }

    // Increment/decrement operators
    constexpr StringView_iterator& operator ++ () {
        ++m_currPtr;

        return *this;
    }
    constexpr StringView_iterator operator ++ (int) {
        return { m_currPtr++, m_endPtr };
    }

    constexpr StringView_iterator& operator -- () {
        --m_currPtr;

        return *this;
    }
    constexpr StringView_iterator operator -- (int) {
        return { m_currPtr--, m_endPtr };
    }

    // Compound assignment operators
    constexpr StringView_iterator& operator += (difference_type delta) {
        m_currPtr += delta;

        return *this;
    }

    constexpr StringView_iterator& operator -= (difference_type delta) {
        m_currPtr -= delta;

        return *this;
    }

    // Arithmetic operators
    constexpr StringView_iterator operator + (difference_type delta) const {
        return { m_currPtr + delta, m_endPtr };
    }

    constexpr StringView_iterator operator - (difference_type delta) const {
        return { m_currPtr - delta, m_endPtr };
    }

    // Iterator distance operator
    constexpr difference_type operator - (const StringView_iterator& other) const {
        return m_currPtr - other.m_currPtr;
    }

    // Comparison operators
    constexpr bool operator == (const StringView_iterator& other) const {
        return strPtr() == other.strPtr();
    }
    constexpr bool operator != (const StringView_iterator& other) const {
        return !(*this == other);
    }
    constexpr bool operator > (const StringView_iterator& other) const {
        return strPtr() > other.strPtr();
    }
    constexpr bool operator < (const StringView_iterator& other) const {
        return other > *this;
    }
    constexpr bool operator >= (const StringView_iterator& other) const {
        return strPtr() >= other.strPtr();
    }
    constexpr bool operator <= (const StringView_iterator& other) const {
        return strPtr() <= other.strPtr();
    }

    // Subscript operator
    constexpr value_type operator [] (difference_type n) const {
        return *(*this + n);
    }

private:
    constexpr pointer strPtr() const {
        return m_currPtr;
    }
    constexpr pointer endPtr() const {
        return m_endPtr;
    }

    pointer m_currPtr;
    pointer m_endPtr;

    friend class ::Util::StringView;
};

} // namespace Internals

class StringView {
public:
    using iterator = Internals::StringView_iterator;

    constexpr StringView() {}
    constexpr StringView(const StringView& other) = default;
    constexpr StringView(StringView&& other) = default;
    constexpr StringView(const char* str, size_t length) :
      m_strPtr{ str },
      m_length{ length }
    {}
    constexpr StringView(iterator startIter, iterator endIter) :
      StringView(startIter.m_currPtr, endIter - startIter)
    {
        assert(startIter <= endIter);
    }
    StringView(const char* str) :
      StringView{ str, std::strlen(str) }
    {}
    StringView(const std::string& str, size_t offset, size_t length) :
      StringView{ str.data() + offset, length }
    {
        assert(offset <= str.size());
        assert(length <= (str.size() - offset));
    }
    StringView(const std::string& str) :
      StringView{ str.data(), str.size() }
    {}

    constexpr StringView& operator = (const StringView& other) = default;
    constexpr StringView& operator = (StringView&& other) = default;

    constexpr const char&  operator [] (size_t index) const {
        return get(index);
    }

    constexpr size_t size() const {
        return m_length;
    }

    constexpr const char* data() const {
        return m_strPtr;
    }

    constexpr char front() const {
        return size() > 0 ? get(0) : '\0';
    }

    constexpr char back() const {
        return size() > 0 ? get(size() - 1) : '\0';
    }

    template <typename Comparator>
    constexpr int compare(StringView other, Comparator&& cmp = Comparator()) const {
        size_t numIters = std::min(size(), other.size());

        iterator iterOne = begin();
        iterator iterTwo = other.begin();

        while (numIters--) {
            int cmpResult = cmp(*iterOne++, *iterTwo++);

            if (cmpResult != 0) {
                return cmpResult;
            }
        }
        return cmp(*iterOne, *iterTwo);
    }

    constexpr int compare(StringView other) const {
        return compare(other, lexicographicCmp);
    }

    template <typename Comparator>
    constexpr bool beginsWith(StringView prefix, Comparator&& cmp = Comparator()) const {
        if (prefix.size() > size()) return false;

        return subString(0, prefix.size()).compare(prefix, cmp) == 0;
    }

    constexpr bool beginsWith(StringView prefix) const {
        return beginsWith(prefix, lexicographicCmp);
    }

    template <typename Comparator>
    constexpr bool endsWith(StringView suffix, Comparator&& cmp = Comparator()) const {
        if (suffix.size() > size()) return false;

        return subString(size() - suffix.size()).compare(suffix, cmp) == 0;
    }

    constexpr bool endsWith(StringView suffix) const {
        return endsWith(suffix, lexicographicCmp);
    }

    constexpr bool operator == (StringView other) const {
        return compare(other) == 0;
    }

    constexpr bool operator != (StringView other) const {
        return !(*this == other);
    }

    constexpr StringView subString(size_t baseOffset, size_t length) const {
        assert(baseOffset <= size());
        assert(length <= (size() - baseOffset));

        return { data() + baseOffset, length };
    }

    constexpr StringView subString(size_t baseOffset) const {
        return subString(baseOffset, size() - baseOffset);
    }

    constexpr iterator begin() const {
        return { data(), data() + size() };
    }
    constexpr iterator end() const {
        auto endPtr = data() + size();

        return { endPtr, endPtr };
    }

    constexpr iterator cbegin() const {
        return begin();
    }
    constexpr iterator cend() const {
        return end();
    }

    template <typename Func>
    static size_t hash(const StringView& strView, Func&& fn = Func()) {
        static_assert(std::is_same_v<char, decltype(std::declval<Func>()(char(0)))>,
            "Invalid type signature.");

        Hash hasher(0xED7CB501657307DFULL, 0xDB0487630191DE0BULL, 1);

        for (char c : strView) {
            hasher(fn(c));
        }
        return hasher.finalize();
    }

    static size_t hash(const StringView& strView) {
        return hash(strView, [](char c) { return c; });
    }

private:
    static constexpr int lexicographicCmp(char c1, char c2) {
        return c1 - c2;
    }

    constexpr const char& get(size_t index) const {
        return *(data() + index);
    }

    const char* m_strPtr = nullptr;
    size_t m_length = 0;
};

inline std::ostream& operator << (std::ostream& outStream, const StringView& strView) {
    outStream.write(strView.data(), strView.size());

    return outStream;
}

} // namespace Util

constexpr Util::StringView operator ""_sv (const char* str, size_t strSize) {
    return { str, strSize };
}

namespace std {

template <>
struct hash<Util::StringView> {
    size_t operator () (const Util::StringView& strView) const {
        return Util::StringView::hash(strView);
    }
};

} // namespace std
