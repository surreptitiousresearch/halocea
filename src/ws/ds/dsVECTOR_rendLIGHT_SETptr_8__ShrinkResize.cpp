#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct rendLIGHT_SET; // boundary — rend light set; element is a pointer.

// dsVECTOR<rendLIGHT_SET*,8>::ShrinkResize @ 0x82ABCF68
// Shrink the live count to `size`, dropping the trailing element slots (no reallocation).
// The element type is a pointer, so the per-slot destructor loop in the original is a no-op.
template<>
void dsVECTOR<rendLIGHT_SET *, 8>::ShrinkResize(int size)
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
