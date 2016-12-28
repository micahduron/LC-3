#include <cstdio>
#include "lc3.h"

#pragma once

class LC3Writer {
public:
    using word_type = LC3::Word;

    LC3Writer() {}
    LC3Writer(const LC3Writer& other) = delete;
    LC3Writer(const char* fileName) :
      m_fileHandle{ LC3Writer::openFile(fileName) }
    {}

    ~LC3Writer() {
        close();
    }

    LC3Writer& operator = (const LC3Writer& other) = delete;

    bool isOpen() const {
        return m_fileHandle != nullptr;
    }

    explicit operator bool() const {
        return isOpen();
    }

    void open(const char* fileName);
    void close();

    LC3Writer& put(word_type word);
    LC3Writer& write(const word_type* wordArray, size_t numWords);

private:
    using file_type = std::FILE*;

    static file_type openFile(const char* fileName) {
        return std::fopen(fileName, "w");
    }

    file_type m_fileHandle = nullptr;
};
