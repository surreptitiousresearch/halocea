#include "dsVECTOR.h"
#include "dsStrIntMapERASED.h"
#include "ds_assert_boundary.h"

// dsVECTOR<...StrInt-map ERASED,8>::operator[] (non-const) @ 0x82616808
// Bounds-checked mutable access; returns a reference to the idx-th deferred-erase record.
template<>
dsStrIntMapERASED &dsVECTOR<dsStrIntMapERASED, 8>::operator[](int idx)
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
