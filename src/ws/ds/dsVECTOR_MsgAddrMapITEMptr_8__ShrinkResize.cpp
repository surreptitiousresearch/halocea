#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::ShrinkResize @ 0x82559130
// Shrink the live count to `size`. The element is a raw pointer (trivially destructible), so the
// generic per-element destructor loop is empty and collapses to clamping the count.
template<>
void dsVECTOR<dsMsgAddrMapITEM *, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size", size);
    if (size < this->nElem)
        this->nElem = size;
}
