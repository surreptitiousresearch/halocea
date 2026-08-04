#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::operator[] (const) @ 0x82558D50
// Bounds-checked element access; returns a const reference to the pointer slot at `idx`.
template<>
dsMsgAddrMapITEM *const &dsVECTOR<dsMsgAddrMapITEM *, 8>::operator[](int idx) const
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
