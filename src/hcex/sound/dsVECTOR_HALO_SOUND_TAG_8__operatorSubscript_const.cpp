#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/hcex/HALO_SOUND_TAG.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<HALO_SOUND_TAG,8>::operator[] (const) @ 0x836BA1A8
// Bounds-checked read-only access; returns a const reference to the idx-th sound-tag entry.
template<>
const HALO_SOUND_TAG &dsVECTOR<HALO_SOUND_TAG, 8>::operator[](int idx) const
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
