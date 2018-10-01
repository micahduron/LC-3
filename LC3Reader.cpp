#include "LC3Reader.h"

bool LC3Reader::getWord(word_type& wordRef) {
    LC3::Word::value_type wordVal;

    if (get(wordVal)) {
        wordRef = wordVal;

        return true;
    }
    return false;
}
