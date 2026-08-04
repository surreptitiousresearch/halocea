#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../../headers/hcex/HCEX_DECAL.h"

// dsVECTOR<HCEX_DECAL,8>::Back @ 0x823D81E0
// Reference to the last live element (index nElem-1); assert non-empty first.
template<>
HCEX_DECAL &dsVECTOR<HCEX_DECAL, 8>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            527,
            "nElem",
            this->nElem);
    return this->pData[this->nElem - 1];
}
