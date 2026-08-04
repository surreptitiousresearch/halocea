#include "dsVECTOR.h"
#include "WEAK_PTR.h"
#include "ds_assert_boundary.h"

struct entENTITY; // boundary — referenced only as the weak-pointer element type.

// dsVECTOR<ds::WEAK_PTR<entENTITY>,8>::operator[] (non-const) @ 0x823DA588
// Bounds-checked mutable access; returns a reference to the idx-th weak reference (an HCEX_OBJ's
// follower entry). Reached via the flattened extern dsVECTOR_WEAK_PTR_entENTITY_8__operatorSubscript.
template<>
ds::WEAK_PTR<entENTITY> &dsVECTOR<ds::WEAK_PTR<entENTITY>, 8>::operator[](int idx)
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
