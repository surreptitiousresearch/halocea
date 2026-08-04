#include "dsVECTOR.h"
#include "../anim/animLOD_LOD_INFO.h"
#include "ds_assert_boundary.h"

// ??A?$dsVECTOR@ULOD_INFO@animLOD@@$07@@QAAAAULOD_INFO@animLOD@@H@Z  (0x82B0D758) — non-const
// bounds-checked element access; returns a reference to the idx-th per-LOD object record.
template<>
animLOD::LOD_INFO &dsVECTOR<animLOD::LOD_INFO, 8>::operator[](int idx)
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
