#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::ShrinkResize @ 0x82A386A0
// Shrink the live count to `size`. NAME_TO_IDX is trivially destructible, so the generic
// per-element destructor loop is empty and collapses to the count.
template<>
void dsVECTOR<ssl_class::NAME_TO_IDX, 8>::ShrinkResize(int size)
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
