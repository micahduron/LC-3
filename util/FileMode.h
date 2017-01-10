#pragma once

namespace Util::FileIO::FileMode {
    struct Writer {
        static std::FILE* open(const char *fileName) {
            return std::fopen(fileName, "wb");
        }
    };
    struct Reader {
        static std::FILE* open(const char *fileName) {
            return std::fopen(fileName, "rb");
        }
    };
}
