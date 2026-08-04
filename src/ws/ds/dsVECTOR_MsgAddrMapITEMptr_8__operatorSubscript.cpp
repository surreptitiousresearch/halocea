#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::operator[] (non-const) @ 0x82558CB0
// Bounds-checked element access; returns a reference to the pointer slot at `idx`.
template<>
dsMsgAddrMapITEM *&dsVECTOR<dsMsgAddrMapITEM *, 8>::operator[](int idx)
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
