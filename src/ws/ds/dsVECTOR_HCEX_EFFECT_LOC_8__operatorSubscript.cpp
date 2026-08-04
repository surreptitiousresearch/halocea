#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_EFFECT_LOC.h"
#include "ds_assert_boundary.h"

// dsVECTOR<HCEX_EFFECT_LOC,8>::operator[] (non-const) @ 0x823DA758
// Bounds-checked mutable access into an effect's per-locator table; returns a reference to the
// idx-th locator.
template<>
HCEX_EFFECT_LOC &dsVECTOR<HCEX_EFFECT_LOC, 8>::operator[](int idx)
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
