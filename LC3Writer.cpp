#include "LC3Writer.h"

LC3Writer& LC3Writer::put(word_type word) {
    return write(&word, 1);
}
LC3Writer& LC3Writer::write(const word_type* wordArray, size_t numWords) {
    std::fwrite(wordArray, sizeof(word_type), numWords, file());

    return *this;
}
