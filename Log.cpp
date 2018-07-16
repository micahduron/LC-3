#include "Log.h"

namespace LC3 {

static size_t numErrors = 0;
static size_t numWarnings = 0;

std::ostream& Log::error() {
    ++numErrors;

    return (std::cerr << "Error: ");
}

size_t Log::errorCount() {
    return numErrors;
}

std::ostream& Log::warning() {
    ++numWarnings;

    return (std::cerr << "Warning: ");
}

size_t Log::warningCount() {
    return numWarnings;
}

} // namespace LC3
