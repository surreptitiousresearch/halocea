#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

class rendLIGHT_SET; // boundary — render-side light-set object (element is a pointer)

// dsVECTOR<rendLIGHT_SET*,8>::operator[] (non-const) @ 0x82ABD170
// Bounds-checked mutable access; returns a reference to the idx-th element pointer.
template<>
rendLIGHT_SET *&dsVECTOR<rendLIGHT_SET *, 8>::operator[](int idx)
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
