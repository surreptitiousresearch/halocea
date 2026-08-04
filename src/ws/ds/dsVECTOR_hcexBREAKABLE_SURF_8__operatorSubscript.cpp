#include "dsVECTOR.h"
#include "../../headers/hcex/hcex_breakable_surf.h"
#include "ds_assert_boundary.h"

// dsVECTOR<hcexBREAKABLE_SURF,8>::operator[] (non-const) @ 0x823B1F68
// Bounds-checked mutable access into the breakable-surface table; returns a reference to the idx-th
// record.
template<>
hcexBREAKABLE_SURF &dsVECTOR<hcexBREAKABLE_SURF, 8>::operator[](int idx)
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
