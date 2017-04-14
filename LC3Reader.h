#include "util/BinaryReader.h"
#include "util/EndiannessConverter.h"
#include "lc3.h"

#pragma once

class LC3Reader :
  private Util::BinaryReader<Util::EndiannessConverter<Util::BigEndian>>
{
public:
    using word_type = LC3::Word;

    using BinaryReader::BinaryReader;

    using BinaryReader::isOpen;
    using BinaryReader::operator bool;

    using BinaryReader::open;
    using BinaryReader::close;

    size_t readWords(word_type* wordBuf, size_t numWords);
    bool getWord(word_type& wordRef);
};
