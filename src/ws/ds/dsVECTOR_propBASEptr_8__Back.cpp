#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

// dsVECTOR<propBASE*,8>::Back @ 0x826C3848
// Reference to the last live element pointer (index nElem-1); assert non-empty first.
template<>
propBASE *&dsVECTOR<propBASE *, 8>::Back()
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
