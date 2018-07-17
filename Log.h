#pragma once

#include <iostream>

namespace LC3 {

class Log {
public:
    static std::ostream& error(bool newError = true);
    static size_t errorCount();

    static std::ostream& warning(bool newWarning = true);
    static size_t warningCount();
};

} // namespace LC3
