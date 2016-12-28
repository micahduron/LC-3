#include <cstdio>

#pragma once

namespace FileMode {
    struct Writer {
        static std::FILE* open(const char *fileName) {
            return std::fopen(fileName, "w");
        }
    };
    struct Reader {
        static std::FILE* open(const char *fileName) {
            return std::fopen(fileName, "r");
        }
    };
};

template <typename Mode>
class BinaryFileIO {
public:
    BinaryFileIO() {}
    BinaryFileIO(const BinaryFileIO& other) = delete;
    BinaryFileIO(const char* fileName) :
      m_fileHandle{ Mode::open(fileName) }
    {}

    ~BinaryFileIO() {
        close();
    }

    BinaryFileIO& operator = (const BinaryFileIO& other) = delete;

    bool isOpen() const {
        return m_fileHandle != nullptr;
    }

    explicit operator bool() const {
        return isOpen();
    }

    void open(const char* fileName);
    void close();

protected:
    using file_type = std::FILE*;

    file_type file() {
        return m_fileHandle;
    }

private:
    std::FILE* m_fileHandle = nullptr;
};

#include "BinaryFileIO.tpp"
