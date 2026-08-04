#include "dsVECTOR.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsDATA,8>::operator[] (non-const) @ 0x825A87B0
// Bounds-checked mutable access; returns a reference to the idx-th element.
template<>
dsDATA &dsVECTOR<dsDATA, 8>::operator[](int idx)
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
