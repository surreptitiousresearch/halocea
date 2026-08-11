#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/hcex/HCEX_EFFECT.h"
#include "../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<HCEX_EFFECT,8>::operator[] (non-const) @ 0x823DAFC0
// Bounds-checked mutable access; returns a reference to the idx-th effect.
template<>
HCEX_EFFECT &dsVECTOR<HCEX_EFFECT, 8>::operator[](int idx)
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
