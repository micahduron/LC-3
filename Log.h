#pragma once

#include <iostream>

namespace LC3 {

class Log {
public:
    static std::ostream& error();
    static size_t errorCount();

    static std::ostream& warning();
    static size_t warningCount();
};

} // namespace LC3
