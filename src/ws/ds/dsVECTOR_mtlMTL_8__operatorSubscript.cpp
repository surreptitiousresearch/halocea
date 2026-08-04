#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"
#include "ds_assert_boundary.h"

// dsVECTOR<mtlMTL,8>::operator[](int) [non-const] @ 0x82685E20
// Reference to the element at `index`. Asserts 0 <= index < nElem.
// NOTE: the decompiler emitted extra phantom int params (a3..a14) that are just the STRONG_ASSERT2
// varargs helper's outgoing arguments folded into the caller's frame by the decompiler; only `index`
// is a real parameter — verified against the two-argument (idx, nElem) assert call.
template<>
mtlMTL &dsVECTOR<mtlMTL, 8>::operator[](int index)
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", index,
            "nElem", this->nElem);
    return this->pData[index];
}
