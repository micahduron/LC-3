#include "BinaryFileIO.h"

template <typename Mode>
void BinaryFileIO<Mode>::open(const char* fileName) {
    close();

    m_fileHandle = Mode::open(fileName);
}
template <typename Mode>
void BinaryFileIO<Mode>::close() {
    if (m_fileHandle) {
        std::fclose(m_fileHandle);

        m_fileHandle = nullptr;
    }
}
