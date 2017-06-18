#include <iostream>
#include <string>
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
    StringView(const std::string& str, size_t length) :
      StringView{ str.data(), length }
    {}

    StringView& operator = (const StringView& other) = default;
    StringView& operator = (StringView&& other) = default;

    const char&  operator [] (size_t index) const {
        return m_strPtr[index];
    }

    size_t size() const {
        return m_length;
    }

    const char* data() const {
        return m_strPtr;
    }

    int compare(const StringView& other, compare_func cmpFn) const;
    int compare(const std::string& other, compare_func cmpFn) const;
    int compare(const char* other, compare_func cmpFn) const;

    int compare(const StringView& other) const;
    int compare(const std::string& other) const;
    int compare(const char* other) const;

    bool operator == (const StringView& other) const;
    bool operator == (const std::string& other) const;
    bool operator == (const char* other) const;

    template <typename T>
    bool operator != (const T& other) const {
        return !(*this == other);
    }

    StringView subString(size_t baseOffset, size_t length) {
        return { m_strPtr + baseOffset, length };
    }

    iterator begin() const;
    iterator end() const;

    iterator cbegin() const;
    iterator cend() const;

private:
    const char* m_strPtr;
    size_t m_length;

    friend std::ostream& operator << (std::ostream&, const StringView&);
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
    reference operator * () const {
        // Because this method returns a reference, I cannot simply return '\0'.
        // Rather, I have to statically allocate an empty string and return its
        // null-terminator.
        static const char emptyString[] = "";
        const char& nullChar = emptyString[0];

        return m_currPtr != m_endPtr ? *m_currPtr : nullChar;
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
};

std::ostream& operator << (std::ostream& outStream, const StringView& strView);

}
