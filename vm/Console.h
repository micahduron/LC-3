#pragma once

namespace LC3::VM {

class Console {
public:
    static void activate();
    static void deactivate();

    static void setState(bool state);
    static bool getState();

    static int readChar();
    static ssize_t readString(char* strBuf, size_t bufSize);
};

} // namespace LC3::VM
