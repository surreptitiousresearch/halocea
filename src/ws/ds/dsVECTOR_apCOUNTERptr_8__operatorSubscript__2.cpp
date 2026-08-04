#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::operator[] (const) @ 0x82715318
// Bounds-checked read-only access; returns a reference to the idx-th element pointer.
template<>
apCOUNTER *const &dsVECTOR<apCOUNTER *, 8>::operator[](int idx) const
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
