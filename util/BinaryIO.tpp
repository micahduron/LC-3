#include "BinaryIO.h"

template <typename Mode>
void BinaryIO<Mode>::open(const char* fileName) {
    close();

    m_fileHandle = Mode::open(fileName);
}
template <typename Mode>
void BinaryIO<Mode>::close() {
    if (isOpen()) {
        std::fclose(m_fileHandle);

        m_fileHandle = nullptr;
    }
}
