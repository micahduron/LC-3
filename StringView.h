#include <string>
#include <utility>

class StringView {
public:
    StringView(const StringView& other) :
     m_strPtr{ other.m_strPtr },
     m_length{ other.m_length }
    {}
    StringView(const std::string& str, size_t startIndex, size_t strLen) :
     m_strPtr{ str.data() + startIndex },
     m_length{ strLen }
    {}
    StringView(const char* str, size_t startIndex, size_t strLen) :
     m_strPtr{ str + startIndex },
     m_length{ strLen }
    {}

    char&        operator [] (size_t index)       { return m_strPtr[index]; }
    const char&  operator [] (size_t index) const { return m_strPtr[index]; }

    size_t size() const { return m_length; }

    iterator       begin() { return iterator{ m_strPtr }; }
    const_iterator begin() const { return const_iterator{ m_strPtr }; }

    iterator       end() { return iterator{ m_strPtr + m_length }; }
    const_iterator end() const { return const_iterator{ m_strPtr + m_length }; }

    const_iterator cbegin() const { return this->begin(); }
    const_iterator cend() const { return this->end(); }

private:
    const char* m_strPtr;
    size_t m_length;
};
