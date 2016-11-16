#include <iostream>
#include <string>
#include <utility>

#pragma once

class StringView {
public:
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

    const char&  operator [] (size_t index) const { return m_strPtr[index]; }

    size_t size() const { return m_length; }

    const char* data() const {
        return m_strPtr;
    }

    iterator begin() const { return iterator{ m_strPtr }; }
    iterator end() const { return iterator{ m_strPtr + m_length }; }

    iterator cbegin() const { return this->begin(); }
    iterator cend() const { return this->end(); }

private:
    const char* m_strPtr;
    size_t m_length;

    friend std::ostream& operator << (std::ostream&, const StringView&);
};

std::ostream& operator << (std::ostream& outStream, const StringView& strView);
