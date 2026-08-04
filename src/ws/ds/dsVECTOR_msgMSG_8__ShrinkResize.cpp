#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../msg/msgMSG.h"

// dsVECTOR<msgMSG,8>::ShrinkResize @ 0x82AC4B30
// Shrink the live count to `size`. msgMSG is a 24-byte POD envelope holding no self-referencing
// pointers, so the per-element destructor loop is empty and collapses to clamping the count.
template<>
void dsVECTOR<msgMSG, 8>::ShrinkResize(int size)
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
