#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../m3d/m3dMATR.h"

// dsVECTOR<m3dMATR,8>::ShrinkResize @ 0x823D8960
// Shrink the live count to `size`. m3dMATR is trivially destructible (plain matrix data),
// so the generic per-element destructor loop has an empty body and collapses to the count.
template<>
void dsVECTOR<m3dMATR, 8>::ShrinkResize(int size)
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
