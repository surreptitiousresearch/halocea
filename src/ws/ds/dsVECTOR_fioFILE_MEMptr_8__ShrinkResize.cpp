#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct fioFILE_MEM; // boundary — fio in-memory file; element is a pointer.

// dsVECTOR<fioFILE_MEM*,8>::ShrinkResize @ 0x82AC8060
// Shrink the live count to `size`, dropping the trailing element slots (no reallocation).
// The element type is a pointer, so the per-slot destructor loop in the original is a no-op.
template<>
void dsVECTOR<fioFILE_MEM *, 8>::ShrinkResize(int size)
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
