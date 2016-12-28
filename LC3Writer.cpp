#include "LC3Writer.h"

void LC3Writer::open(const char* fileName) {
    close();

    m_fileHandle = LC3Writer::openFile(fileName);
}
void LC3Writer::close() {
    if (m_fileHandle) {
        std::fclose(m_fileHandle);
    }
}

LC3Writer& LC3Writer::put(word_type word) {
    auto bytesLeft = sizeof(word_type);
    word_type byteMask = 0xFF << (8 * (sizeof(word_type) - 1));

    while (bytesLeft--) {
        int currByte = (word & byteMask) >> (8 * bytesLeft);
        std::fputc(currByte, m_fileHandle);

        byteMask >>= 8;
    }
    return *this;
}
