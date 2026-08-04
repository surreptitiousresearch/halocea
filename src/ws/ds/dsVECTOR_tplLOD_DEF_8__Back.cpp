#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"
#include "ds_assert_boundary.h"

// dsVECTOR<tplLOD_DEF,8>::Back @ 0x82B0D3D0
// Reference to the last live element (index nElem-1); assert non-empty first.
template<>
tplLOD_DEF &dsVECTOR<tplLOD_DEF, 8>::Back()
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
