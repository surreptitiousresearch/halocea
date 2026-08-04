#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "dsMsgAddrMapDUMMY.h"

// dsVECTOR<...msgADDR-map DUMMY*,8>::ShrinkResize @ 0x82558FB0
// Shrink the live count to `size`, dropping the trailing pointer slots (no reallocation). The
// element type is a raw pointer, so the per-slot destructor loop in the original is a no-op.
template<>
void dsVECTOR<dsMsgAddrMapDUMMY *, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i) // trivial pointer destructor (empty loop)
            ;
        this->nElem = size;
    }
}
