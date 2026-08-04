#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"
#include "ds_assert_boundary.h"

// dsVECTOR<psSECTION_RECORD,8>::operator[] const @ 0x825186F0
// Bounds-checked element access (const overload).
// DEVIATION: the decompile fabricates 12 phantom trailing int params (a3..a14) from a
// float-slot-skip ABI mis-shift; the real signature (matching the non-const twin at 0x82518650
// and the header's declared `operator[](int) const`) takes just the index.
template<>
const psSECTION_RECORD &dsVECTOR<psSECTION_RECORD, 8>::operator[](int index) const
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", index,
            "nElem", this->nElem);
    return this->pData[index];
}
