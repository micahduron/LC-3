#include "LC3Reader.h"

bool LC3Reader::getWord(word_type& wordRef) {
    return get(wordRef);
}
size_t LC3Reader::readWords(word_type* wordBuf, size_t numWords) {
    return read(wordBuf, numWords);
}
