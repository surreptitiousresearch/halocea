#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/hcex/HALO_PERM_SOUND.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<HALO_PERM_SOUND,8>::operator[] (non-const) @ 0x836BA638
// Bounds-checked mutable access; returns a reference to the idx-th permutation-sound entry.
template<>
HALO_PERM_SOUND &dsVECTOR<HALO_PERM_SOUND, 8>::operator[](int idx)
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
