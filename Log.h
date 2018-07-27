#pragma once

#include <iostream>

namespace LC3 {

class Log {
public:
    template <typename T>
    static std::ostream& error(const T& errObj) {
        return errObj.errorPrefix(Log::error());
    }

    static std::ostream& error(bool newError = true);
    static size_t errorCount();

    template <typename T>
    static std::ostream& warning(const T& warnObj) {
        return warnObj.errorPrefix(Log::warning());
    }

    static std::ostream& warning(bool newWarning = true);
    static size_t warningCount();
};

} // namespace LC3
