#include "dsVECTOR.h"
#include "../ssl/sslFUNCTION_DECL.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslFUNCTION_DECL::PARAM,8>::operator[] (non-const) @ 0x82A38608
// Bounds-checked mutable access; returns a reference to element `idx`.
template<>
sslFUNCTION_DECL::PARAM &dsVECTOR<sslFUNCTION_DECL::PARAM, 8>::operator[](int idx)
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
