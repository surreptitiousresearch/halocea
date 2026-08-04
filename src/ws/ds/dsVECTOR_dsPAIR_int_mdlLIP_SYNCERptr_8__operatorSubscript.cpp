#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "ds_assert_boundary.h"

struct mdlLIP_SYNCER; // boundary — ws-engine animation lip-syncer (element holds a pointer)

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::operator[] (non-const) @ 0x823DADB0
// Bounds-checked mutable access; returns a reference to the idx-th id/lip-syncer pair.
template<>
dsPAIR<int, mdlLIP_SYNCER *> &dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::operator[](int idx)
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
