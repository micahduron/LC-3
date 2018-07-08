#pragma once

#include <cstdint>

namespace Util {

class Hash {
public:
    Hash(const Hash& other) = default;
    Hash(Hash&& other) = default;
    Hash(uintmax_t accumulatorMult, uintmax_t finalMult, size_t shiftFactor) :
      m_accMultInit{ accumulatorMult },
      m_finalMult{ finalMult },
      m_shiftVal{ shiftFactor },
      m_accMult{ m_accMultInit }
    {}

    Hash& operator = (const Hash& other) = delete;

    template <typename T>
    void operator () (const T& obj) {
        size_t objHash = std::hash<T>()(obj);
        m_accumulator += objHash * m_accMult;

        m_accMult *= m_accMultInit;
    }

    size_t finalize() {
        uintmax_t preHash = (m_accumulator >> m_shiftVal) * m_finalMult;

        m_accumulator = 0;
        m_accMult = m_accMultInit;

        return static_cast<size_t>(preHash);
    }

private:
    const uintmax_t m_accMultInit;
    const uintmax_t m_finalMult;
    const size_t m_shiftVal;

    uintmax_t m_accumulator = 0;
    uintmax_t m_accMult;
};

} // namespace Util
