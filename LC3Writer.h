#include <cstdio>
#include "lc3.h"

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

    explicit operator bool() const {
        return m_fileHandle != nullptr;
    }

    void open(const char* fileName);
    void close();

    LC3Writer& put(word_type word);

private:
    using file_type = std::FILE*;

    static file_type openFile(const char* fileName) {
        return std::fopen(fileName, "w");
    }

    file_type m_fileHandle = nullptr;
};
