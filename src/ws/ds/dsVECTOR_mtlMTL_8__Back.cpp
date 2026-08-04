#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"
#include "ds_assert_boundary.h"

// dsVECTOR<mtlMTL,8>::Back @ 0x82C2B180
// Reference to the last live element. Asserts nElem > 0.
template<>
mtlMTL &dsVECTOR<mtlMTL, 8>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            527,
            "nElem", this->nElem);
    return this->pData[this->nElem - 1];
}
