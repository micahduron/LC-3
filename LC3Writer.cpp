#include "LC3Writer.h"

void LC3Writer::open(const char* fileName) {
    close();

    m_fileHandle = LC3Writer::openFile(fileName);
}
void LC3Writer::close() {
    if (m_fileHandle) {
        std::fclose(m_fileHandle);

        m_fileHandle = nullptr;
    }
}

LC3Writer& LC3Writer::put(word_type word) {
    return write(&word, 1);
}
LC3Writer& LC3Writer::write(const word_type* wordArray, size_t numWords) {
    std::fwrite(wordArray, sizeof(word_type), numWords, m_fileHandle);

    return *this;
}
