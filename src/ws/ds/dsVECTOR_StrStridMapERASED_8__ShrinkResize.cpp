#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "dsStrStridMapERASED.h"

// dsVECTOR<dsERASE_MANAGER<...dsSTRID-map LINE_ERASER>::ERASED,8>::ShrinkResize @ 0x8252A3A0
// Shrink the live count to `size`, destroying every element from `size` up to the old count.
// ERASED is a trivial POD record, so the emitted destructor loop has an empty body.
template<>
void dsVECTOR<dsStrStridMapERASED, 8>::ShrinkResize(int size)
{
    if (!IGNORE_STRONG_ASSERT && size < 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "size >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            270,
            "size",
            size);
    if (size < this->nElem) {
        for (int i = size; i < this->nElem; ++i)
            this->pData[i].~dsStrStridMapERASED();
        this->nElem = size;
    }
}
