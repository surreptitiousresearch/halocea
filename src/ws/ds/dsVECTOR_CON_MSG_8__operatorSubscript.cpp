#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../con_/CON_MSG.h"

// dsVECTOR<con::CON_MSG,8>::operator[] (non-const) @ 0x8309B0F0
// Bounds-checked mutable access; returns a reference to the idx-th CON_MSG.
template<>
con::CON_MSG &dsVECTOR<con::CON_MSG, 8>::operator[](int idx)
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
