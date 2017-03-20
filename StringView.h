#include <iostream>
#include <string>
#include <utility>
#include <functional>
#include "StringViewIterator.h"

#pragma once

class StringView {
public:
    using iterator = StringViewIterator;

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

    iterator begin() const {
        return { m_strPtr, m_strPtr + m_length };
    }
    iterator end() const {
        auto endPtr = m_strPtr + m_length;

        return { endPtr, endPtr };
    }

    iterator cbegin() const {
        return begin();
    }
    iterator cend() const {
        return end();
    }

private:
    const char* m_strPtr;
    size_t m_length;

    friend std::ostream& operator << (std::ostream&, const StringView&);
};

std::ostream& operator << (std::ostream& outStream, const StringView& strView);
