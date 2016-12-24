#include <iostream>
#include <string>
#include <utility>
#include <functional>

#pragma once

class StringView {
public:
    class iterator;

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

    int compare(const StringView& other, std::function<int(char, char)> cmpFn) const;
    int compare(const std::string& other, std::function<int(char, char)> cmpFn) const;
    int compare(const char* other, std::function<int(char, char)> cmpFn) const;

    StringView subString(size_t baseOffset, size_t length) {
        return { m_strPtr + baseOffset, length };
    }

    iterator begin() const {
        return iterator{ m_strPtr };
    }
    iterator end() const {
        return iterator{ m_strPtr + m_length };
    }

    iterator cbegin() const {
        return begin();
    }
    iterator cend() const {
        return end();
    }

    class iterator :
      public std::iterator<std::input_iterator_tag, const char>
    {
    public:
        iterator(pointer basePtr) :
          m_ptr{ basePtr }
        {}

        iterator operator ++ (int) {
            iterator ret{ *this };

            return ++ret;
        }
        iterator& operator ++ () {
            ++m_ptr;

            return *this;
        }
        bool operator == (iterator other) const {
            return m_ptr == other.m_ptr;
        }
        bool operator != (iterator other) const {
            return !(*this == other);
        }
        reference operator * () const {
            return *m_ptr;
        }
    private:
        pointer m_ptr;
    };

private:
    const char* m_strPtr;
    size_t m_length;

    friend std::ostream& operator << (std::ostream&, const StringView&);
};

std::ostream& operator << (std::ostream& outStream, const StringView& strView);
