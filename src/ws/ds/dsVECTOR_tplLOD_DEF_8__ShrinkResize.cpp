#include "dsVECTOR.h"
#include "../anim/tplLOD_DEF.h"
#include "ds_assert_boundary.h"

// dsVECTOR<tplLOD_DEF,8>::ShrinkResize @ 0x82B0D338
// Shrink the live count to `size`. tplLOD_DEF is trivially destructible (a plain POD record), so
// the generic per-element destructor loop is empty and collapses to the count.
template<>
void dsVECTOR<tplLOD_DEF, 8>::ShrinkResize(int size)
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
