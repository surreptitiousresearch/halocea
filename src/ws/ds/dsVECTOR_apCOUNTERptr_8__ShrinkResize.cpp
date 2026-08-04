#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::ShrinkResize @ 0x827149B0
// Shrink the live count to `size`, dropping the trailing element slots (no reallocation).
// The element type is a pointer, so the per-slot destructor loop in the original is a no-op.
template<>
void dsVECTOR<apCOUNTER *, 8>::ShrinkResize(int size)
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
