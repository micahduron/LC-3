#include "LC3Writer.h"

bool LC3Writer::putWord(word_type word) {
    return put(word);
}
size_t LC3Writer::writeWords(const word_type* wordArray, size_t numWords) {
    return write(wordArray, numWords);
}
