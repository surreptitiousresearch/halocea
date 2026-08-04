#include "dsVECTOR.h"
#include "hcexCINE_INFO.h"
#include "ds_assert_boundary.h"

// dsVECTOR<hcexCINE_INFO,8>::operator[] (non-const) @ 0x823B6888
// Bounds-checked mutable access into a listCine entry's per-difficulty actor/mask table; returns a
// reference to the idx-th record.
template<>
hcexCINE_INFO &dsVECTOR<hcexCINE_INFO, 8>::operator[](int idx)
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
