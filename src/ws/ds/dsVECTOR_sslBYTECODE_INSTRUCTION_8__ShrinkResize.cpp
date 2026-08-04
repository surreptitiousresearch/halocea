#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::ShrinkResize @ 0x830888B0
// Shrink the live count to `size`. sslBYTECODE_INSTRUCTION is trivially destructible, so the
// generic per-element destructor loop is empty and collapses to the count.
template<>
void dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::ShrinkResize(int size)
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
