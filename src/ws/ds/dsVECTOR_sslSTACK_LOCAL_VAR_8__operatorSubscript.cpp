#include "dsVECTOR.h"
#include "../ssl/sslSTACK.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslSTACK::LOCAL_VAR,8>::operator[] (non-const) @ 0x82AF14B0
// Bounds-checked mutable access; returns a reference to element `idx`.
template<>
sslSTACK::LOCAL_VAR &dsVECTOR<sslSTACK::LOCAL_VAR, 8>::operator[](int idx)
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
