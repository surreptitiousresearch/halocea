#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

// dsVECTOR<camCAMERA*,8>::Back @ 0x82703F10
// Reference to the last live element pointer (index nElem-1); assert non-empty first.
template<>
camCAMERA *&dsVECTOR<camCAMERA *, 8>::Back()
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
