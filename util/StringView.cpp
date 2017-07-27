#include "StringView.h"

namespace Util {

std::ostream& operator << (std::ostream& outStream, const StringView& strView) {
    outStream.write(strView.data(), strView.size());

    return outStream;
}

}
