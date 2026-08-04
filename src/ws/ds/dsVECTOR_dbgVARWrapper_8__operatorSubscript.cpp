#include "dsVECTOR.h"
#include "../wb/dbgVARWrapper.h"
#include "ds_assert_boundary.h"

// dsVECTOR<wb::dbgVARWrapper,8>::operator[] (non-const) @ 0x8269DE70
// Bounds-checked mutable access; returns a reference to the idx-th wrapper.
template<>
wb::dbgVARWrapper &dsVECTOR<wb::dbgVARWrapper, 8>::operator[](int idx)
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
