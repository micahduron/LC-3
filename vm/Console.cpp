#include <termios.h>
#include <unistd.h>
#include "Console.h"

namespace LC3::VM {

class Console_Impl {
public:
    Console_Impl() = default;
    Console_Impl(const Console_Impl&) = delete;
    Console_Impl(Console_Impl&&) = delete;

    ~Console_Impl() {
        deactivate();
    }

    Console_Impl& operator = (const Console_Impl&) = delete;
    Console_Impl& operator = (Console_Impl&&) = delete;

    void activate();
    void deactivate();

    bool isActivated() const {
        return m_isActivated;
    }

private:
    bool m_isActivated = false;
    struct termios m_oldSettings;
};

void Console_Impl::activate() {
    tcgetattr(STDIN_FILENO, &m_oldSettings);

    struct termios rawSettings = m_oldSettings;
    rawSettings.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawSettings);

    m_isActivated = true;
}

void Console_Impl::deactivate() {
    if (!m_isActivated) {
        return;
    }
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_oldSettings);

    m_isActivated = false;
}

void Console::setState(bool state) {
    if (state) {
        Console::activate();
    } else {
        Console::deactivate();
    }
}

int Console::readChar() {
    constexpr ssize_t bufSize = 1;
    char readBuf[bufSize + 1];

    if (Console::readString(readBuf, bufSize) != bufSize) {
        return -1;
    }
    return readBuf[0];
}

ssize_t Console::readString(char* strBuf, size_t bufSize) {
    return read(STDIN_FILENO, strBuf, bufSize);
}

static Console_Impl g_console;

void Console::activate() {
    g_console.activate();
}

void Console::deactivate() {
    g_console.deactivate();
}

bool Console::getState() {
    return g_console.isActivated();
}

} // namespace LC3::VM
