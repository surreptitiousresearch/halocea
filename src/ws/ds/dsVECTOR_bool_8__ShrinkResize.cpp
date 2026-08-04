#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// dsVECTOR<bool,8>::ShrinkResize @ 0x82522488
// Shrink the live count to `size`. `bool` is trivially destructible, so the generic
// per-element destructor loop has an empty body and collapses to resetting the count.
template<>
void dsVECTOR<bool, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem)
        this->nElem = size;
}
