#include "util/BinaryFileIO.h"
#include "lc3.h"

#pragma once

class LC3Writer : public BinaryFileIO<FileMode::Writer> {
public:
    using word_type = LC3::Word;

    LC3Writer(const char* fileName) :
      BinaryFileIO{ fileName }
    {}

    LC3Writer& put(word_type word);
    LC3Writer& write(const word_type* wordArray, size_t numWords);
};
