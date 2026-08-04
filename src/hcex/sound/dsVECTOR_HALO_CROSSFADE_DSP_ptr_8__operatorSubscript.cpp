#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/hcex/HALO_CROSSFADE_DSP.h"
#include "../../headers/ws/ds/ds_assert_boundary.h"

// dsVECTOR<HALO_CROSSFADE_DSP*,8>::operator[] (non-const) @ 0x836BA408
// Bounds-checked mutable access into the HALO_CROSSFADE_DSP free-list cache; returns a reference
// to the idx-th pointer slot. Element type is a pointer, so the result is a `pointer&`.
// DEVIATION: the decompiler renders the reference return as `**` and returns `&pData[idx]`; the
// mangle's `AAPAU...@@` decodes as a reference-to-pointer, matching `return pData[idx];`.
template<>
HALO_CROSSFADE_DSP *&dsVECTOR<HALO_CROSSFADE_DSP *, 8>::operator[](int idx)
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
