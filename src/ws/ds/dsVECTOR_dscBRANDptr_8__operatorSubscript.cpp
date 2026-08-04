#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

// dsVECTOR<dscBRAND*,8>::operator[] (const) @ 0x82648620
// Bounds-checked read-only access; returns a const reference to the idx-th element pointer.
template<>
dscBRAND *const &dsVECTOR<dscBRAND *, 8>::operator[](int idx) const
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
