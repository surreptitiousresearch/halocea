#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::operator[] (non-const) @ 0x82A383A8
// Bounds-checked mutable access; returns a reference to element `idx`. (The decompiler's trailing
// phantom float-ABI args are dropped.)
template<>
ssl_class::NAME_TO_IDX &dsVECTOR<ssl_class::NAME_TO_IDX, 8>::operator[](int idx)
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
