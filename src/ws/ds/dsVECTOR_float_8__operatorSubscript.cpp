#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<float,8>::operator[] (non-const) @ 0x82715610
// Bounds-checked mutable access; returns a reference to the idx-th element. (A separate const
// overload also exists at 0x82945B88, QBAABMH@Z — not part of this batch's target list.)
template<>
float &dsVECTOR<float, 8>::operator[](int idx)
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
