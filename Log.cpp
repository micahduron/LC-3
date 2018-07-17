#include "Log.h"

namespace LC3 {

static size_t numErrors = 0;
static size_t numWarnings = 0;

std::ostream& Log::error(bool newError) {
    if (newError) {
        ++numErrors;
        std::cerr << "Error: ";
    }
    return std::cerr;
}

size_t Log::errorCount() {
    return numErrors;
}

std::ostream& Log::warning(bool newWarning) {
    if (newWarning) {
        ++numWarnings;
        std::cerr << "Warning: ";
    }
    return std::cerr;
}

size_t Log::warningCount() {
    return numWarnings;
}

} // namespace LC3
