#include "dsVECTOR.h"
#include "hcexCINE_INFO.h"
#include "ds_assert_boundary.h"

// dsVECTOR<hcexCINE_INFO,8>::Back @ 0x823B6808
// Reference to the last live record (index nElem-1) of a per-difficulty actor/mask table; assert
// non-empty first.
template<>
hcexCINE_INFO &dsVECTOR<hcexCINE_INFO, 8>::Back()
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
