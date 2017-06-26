#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <utility>
#include <functional>

#pragma once

namespace Util {

class StringView {
public:
    class iterator;

    using compare_func = std::function<int(char, char)>;

    StringView(const StringView& other) = default;
    StringView(StringView&& other) = default;
    StringView(const char* str, size_t length) :
      m_strPtr{ str },
      m_length{ length }
    {}
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
    StringView(iterator startIter, iterator endIter);

    StringView& operator = (const StringView& other) = default;
    StringView& operator = (StringView&& other) = default;

    const char&  operator [] (size_t index) const {
        return *(data() + index);
    }

    size_t size() const {
        return m_length;
    }

    const char* data() const {
        return m_strPtr;
    }

    int compare(StringView other, compare_func cmpFn) const;

    int compare(StringView other) const;

    bool operator == (StringView other) const;

    bool operator != (StringView other) const {
        return !(*this == other);
    }

    StringView subString(size_t baseOffset, size_t length) {
        assert(baseOffset <= size());
        assert(length <= (size() - baseOffset));

        return { data() + baseOffset, length };
    }

    iterator begin() const;
    iterator end() const;

    iterator cbegin() const;
    iterator cend() const;

private:
    const char* m_strPtr;
    size_t m_length;
};

class StringView::iterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = const char*;
    using reference = const char&;
    using iterator_category = std::random_access_iterator_tag;

    iterator(const iterator& other) = default;
    iterator(iterator&& other) = default;

    iterator(const char* currPtr, const char* endPtr) :
      m_currPtr{ currPtr },
      m_endPtr{ endPtr }
    {}

    iterator& operator = (const iterator& other) = default;
    iterator& operator = (iterator&& other) = default;

    // Pointer dereference operator
    value_type operator * () const {
        return m_currPtr != m_endPtr ? *m_currPtr : '\0';
    }

    // Increment/decrement operators
    iterator& operator ++ () {
        ++m_currPtr;

        return *this;
    }
    iterator operator ++ (int) {
        return { m_currPtr++, m_endPtr };
    }

    iterator& operator -- () {
        --m_currPtr;

        return *this;
    }
    iterator operator -- (int) {
        return { m_currPtr--, m_endPtr };
    }

    // Compound assignment operators
    iterator& operator += (difference_type delta) {
        m_currPtr += delta;

        return *this;
    }

    iterator& operator -= (difference_type delta) {
        m_currPtr -= delta;

        return *this;
    }

    // Arithmetic operators
    iterator operator + (difference_type delta) const {
        return { m_currPtr + delta, m_endPtr };
    }

    iterator operator - (difference_type delta) const {
        return { m_currPtr - delta, m_endPtr };
    }

    // Iterator distance operator
    difference_type operator - (const iterator& other) const {
        return m_currPtr - other.m_currPtr;
    }

    // Comparison operators
    bool operator == (const iterator& other) const {
        return m_currPtr == other.m_currPtr;
    }
    bool operator != (const iterator& other) const {
        return !(*this == other);
    }
    bool operator > (const iterator& other) const {
        return (*this - other) > 0;
    }
    bool operator < (const iterator& other) const {
        return other > *this;
    }
    bool operator >= (const iterator& other) const {
        return (*this == other) || (*this > other);
    }
    bool operator <= (const iterator& other) const {
        return (*this == other) || (*this < other);
    }

    // Subscript operator
    value_type operator [] (difference_type n) const {
        return *(*this + n);
    }
private:
    pointer m_currPtr;
    pointer m_endPtr;

    friend class StringView;
};

std::ostream& operator << (std::ostream& outStream, const StringView& strView);

}
