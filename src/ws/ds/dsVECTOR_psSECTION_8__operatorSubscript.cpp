#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "psSECTION.h"

// dsVECTOR<psSECTION,8>::operator[](int) @ 0x825183E0 (non-const)
// Bounds-checked element access; asserts 0 <= idx < nElem.
template<>
psSECTION &dsVECTOR<psSECTION, 8>::operator[](int idx)
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
