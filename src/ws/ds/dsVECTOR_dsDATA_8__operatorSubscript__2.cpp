#include "dsVECTOR.h"
#include "dsDATA.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsDATA,8>::operator[] (const) @ 0x825A8848
// Bounds-checked read-only access; returns a const reference to the idx-th element.
template<>
const dsDATA &dsVECTOR<dsDATA, 8>::operator[](int idx) const
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
