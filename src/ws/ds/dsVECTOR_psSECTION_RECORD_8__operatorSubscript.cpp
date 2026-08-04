#include "dsVECTOR.h"
#include "../ps/psSECTION_RECORD.h"
#include "ds_assert_boundary.h"

// dsVECTOR<psSECTION_RECORD,8>::operator[] (non-const) @ 0x82518650
// Bounds-checked element access.
// DEVIATION: the decompile fabricates 12 phantom trailing int params (a3..a14) from a
// float-slot-skip ABI mis-shift; the real signature (matching the const twin at 0x825186F0 and
// the header's declared `operator[](int)`) takes just the index.
template<>
psSECTION_RECORD &dsVECTOR<psSECTION_RECORD, 8>::operator[](int index)
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", index,
            "nElem", this->nElem);
    return this->pData[index];
}
