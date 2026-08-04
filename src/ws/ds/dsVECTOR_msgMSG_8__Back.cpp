#include "dsVECTOR.h"
#include "../msg/msgMSG.h"
#include "ds_assert_boundary.h"

// dsVECTOR<msgMSG,8>::Back @ 0x82AC4BF0
// Reference to the last live message envelope (index nElem-1); assert non-empty first.
template<>
msgMSG &dsVECTOR<msgMSG, 8>::Back()
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
