class StringViewIterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = const char*;
    using reference = const char&;
    using iterator_category = std::random_access_iterator_tag;

    StringViewIterator(const StringViewIterator& other) = default;
    StringViewIterator(StringViewIterator&& other) = default;

    StringViewIterator(const char* currPtr, const char* endPtr) :
      m_currPtr{ currPtr },
      m_endPtr{ endPtr }
    {}

    StringViewIterator& operator = (const StringViewIterator& other) = default;
    StringViewIterator& operator = (StringViewIterator&& other) = default;

    // Pointer dereference operator
    value_type operator * () const {
        return m_currPtr != m_endPtr ? *m_currPtr : '\0';
    }

    // Increment/decrement operators
    StringViewIterator& operator ++ () {
        ++m_currPtr;

        return *this;
    }
    StringViewIterator operator ++ (int) {
        return { ++m_currPtr, m_endPtr };
    }

    StringViewIterator& operator -- () {
        --m_currPtr;

        return *this;
    }
    StringViewIterator operator -- (int) {
        return { m_currPtr--, m_endPtr };
    }

    // Compound assignment operators
    StringViewIterator& operator += (difference_type delta) {
        m_currPtr += delta;

        return *this;
    }

    StringViewIterator& operator -= (difference_type delta) {
        m_currPtr -= delta;

        return *this;
    }

    // Arithmetic operators
    StringViewIterator operator + (difference_type delta) const {
        return { m_currPtr + delta, m_endPtr };
    }

    StringViewIterator operator - (difference_type delta) const {
        return { m_currPtr - delta, m_endPtr };
    }

    // Iterator distance operator
    difference_type operator - (const StringViewIterator& other) const {
        return m_currPtr - other.m_currPtr;
    }

    // Comparison operators
    bool operator == (const StringViewIterator& other) const {
        return m_currPtr == other.m_currPtr;
    }
    bool operator != (const StringViewIterator& other) const {
        return !(*this == other);
    }
    bool operator > (const StringViewIterator& other) const {
        return (*this - other) > 0;
    }
    bool operator < (const StringViewIterator& other) const {
        return other > *this;
    }
    bool operator >= (const StringViewIterator& other) const {
        return (*this == other) || (*this > other);
    }
    bool operator <= (const StringViewIterator& other) const {
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
