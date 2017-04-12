#pragma once

namespace Util::FileIO::FileMode {
    struct Writer {
        static constexpr auto mode = "wb";
    };
    struct Reader {
        static constexpr auto mode = "rb";
    };
}
