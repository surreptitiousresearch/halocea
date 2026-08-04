#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<bool,8>::operator[] (non-const) @ 0x82522520
// Bounds-checked mutable access; returns a reference to the idx-th element.
template<>
bool &dsVECTOR<bool, 8>::operator[](int idx)
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
