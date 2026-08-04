#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "dsMsgAddrMapDUMMY.h"

// dsVECTOR<...msgADDR-map DUMMY*,8>::operator[] @ 0x82558DF0
// Bounds-checked element access; returns a reference to the pointer slot at `idx`. Only the
// non-const overload was found for this instantiation (no `QBA`-mangled sibling in the DB).
template<>
dsMsgAddrMapDUMMY *&dsVECTOR<dsMsgAddrMapDUMMY *, 8>::operator[](int idx)
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
