#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::ShrinkResize @ 0x823DA0E8
// Shrink the live count to `size`. dsSTRID is trivially destructible (a single interned
// const char*), so the generic per-element destructor loop is empty and collapses to the count.
template<>
void dsVECTOR<dsSTRID, 8>::ShrinkResize(int size)
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
