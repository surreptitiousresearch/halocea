#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

// dsVECTOR<dscBRAND*,8>::Back @ 0x826640F0
// Reference to the last live element pointer (index nElem-1); assert non-empty first.
template<>
dscBRAND *&dsVECTOR<dscBRAND *, 8>::Back()
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
