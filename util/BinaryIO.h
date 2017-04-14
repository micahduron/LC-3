#include <cstdio>
#include "FileMode.h"

#pragma once

namespace Util {
    enum SeekMode {
        FromStart = SEEK_SET,
        FromCurrent = SEEK_CUR,
        FromEnd = SEEK_END
    };

    template <typename Mode>
    class BinaryIO {
        static_assert(FileModeTraits<Mode>::IsBinary);

    public:
        BinaryIO() {}
        BinaryIO(const BinaryIO& other) = delete;
        BinaryIO(const char* fileName) :
          m_fileHandle{ BinaryIO::openFile(fileName) }
        {}

        ~BinaryIO() {
            close();
        }

        BinaryIO& operator = (const BinaryIO& other) = delete;

        bool isOpen() const {
            return m_fileHandle != nullptr;
        }

        explicit operator bool() const {
            return isOpen();
        }

        void open(const char* fileName);
        void close();

        bool eof() const {
            return std::feof(const_cast<file_type*>(file()));
        }

        long tell() const {
            return std::ftell(const_cast<file_type*>(file()));
        }
        bool seek(long offset, enum SeekMode mode = SeekMode::FromStart) {
            return std::fseek(file(), offset, mode) == 0;
        }

    protected:
        using file_type = std::FILE;

        file_type* file() {
            return m_fileHandle;
        }
        const file_type* file() const {
            return m_fileHandle;
        }

    private:
        file_type* m_fileHandle = nullptr;

        static file_type* openFile(const char* fileName) {
            return std::fopen(fileName, FileModeTraits<Mode>::ModeString);
        }
    };

#include "BinaryIO.tpp"
}
