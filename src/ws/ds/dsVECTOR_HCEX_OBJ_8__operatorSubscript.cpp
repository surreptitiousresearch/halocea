#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_OBJ.h"

// dsVECTOR<HCEX_OBJ,8>::operator[] (non-const) @ 0x823EDB38
// Bounds-checked mutable access; returns a reference to the idx-th object entry. Reached through
// two flattened externs from the hcex bridge (dsVECTOR_HCEX_OBJ_8__operatorSubscript and dsVECTOR_OBJ_index) —
// both resolve to this single instantiation.
template<>
HCEX_OBJ &dsVECTOR<HCEX_OBJ, 8>::operator[](int idx)
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
