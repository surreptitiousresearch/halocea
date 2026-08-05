#include "dsVECTOR.h"

// boundary — dsERASE_MANAGER<dsLF_SORTED_MAP<...>::LINE_ERASER>::ERASED, a deferred-erase
// bookkeeping record. DB-verified size 12 ({threadMask, eraser.line, eraser.index}); given a
// complete opaque body so the element can be indexed/returned by reference under strict C++.
struct ERASED { unsigned char _opaque[12]; };

// Strong-assert boundary (shared ws-engine debug assert path).
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
struct STRONG_ASSERT2_HELPER {
    template<typename T>
    static void asserd(const char *expr, const char *file, int line,
                       const char *valueName, const T &value); // boundary
};

// dsVECTOR<...ERASED,8>::Front @ 0x8252A2A8
// Return a reference to the first live element; assert the vector is non-empty first.
template<>
ERASED &dsVECTOR<ERASED, 8>::Front()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            507,
            "nElem",
            this->nElem);
    return this->pData[0];
}
