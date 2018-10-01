#include "LC3Writer.h"

bool LC3Writer::putWord(word_type word) {
    return putWord(word.value());
}

bool LC3Writer::putWord(word_type::value_type wordVal) {
    return put(wordVal);
}
