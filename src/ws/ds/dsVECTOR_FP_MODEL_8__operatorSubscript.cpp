#include "dsVECTOR.h"
#include "../../headers/fp_model_vector.h"
#include "ds_assert_boundary.h"

// dsVECTOR<FP_MODEL,8>::operator[] (non-const) @ 0x823DA8C8
// Bounds-checked mutable access into a first-person model list; returns a reference to the idx-th
// record.
template<>
FP_MODEL &dsVECTOR<FP_MODEL, 8>::operator[](int idx)
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
