#include <iostream>

#pragma once

namespace LC3::Language {

class MessageLog {
public:
    std::ostream& getOutputStream() {
        return std::cerr;
    }

    std::ostream& error() {
        ++m_numErrors;

        return getOutputStream() << "\nError: ";
    }

    std::ostream& warning() {
        ++m_numWarnings;

        return getOutputStream() << "\nWarning: ";
    }

    int errorCount() const {
        return m_numErrors;
    }

    int warningCount() const {
        return m_numWarnings;
    }

private:
    int m_numErrors = 0;
    int m_numWarnings = 0;
};

} // namespace LC3::Language
