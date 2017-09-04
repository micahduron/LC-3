#include <util/BinaryWriter.h>
#include <util/EndiannessConverter.h>
#include "lc3.h"

#pragma once

class LC3Writer :
  private Util::BinaryWriter<Util::EndiannessConverter<Util::BigEndian>>
{
public:
    using word_type = LC3::Word;

    using BinaryWriter::BinaryWriter;

    using BinaryWriter::isOpen;
    using BinaryWriter::operator bool;

    using BinaryWriter::open;
    using BinaryWriter::close;

    size_t writeWords(const word_type* wordArray, size_t numWords);
    bool putWord(word_type word);
};
