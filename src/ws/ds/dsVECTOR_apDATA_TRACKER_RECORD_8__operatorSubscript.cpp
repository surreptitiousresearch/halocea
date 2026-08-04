#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../ap/apDATA_TRACKER.h"

// dsVECTOR<apDATA_TRACKER::RECORD,8>::operator[] (non-const) @ 0x82A248A0
// Bounds-checked mutable access; returns a reference to the idx-th record.
// Deviation: the decompiler fabricated a3..a14 phantom parameters and used `a14` as an aliased
// copy of idx (the assert helper takes the argument by address). Real signature is (int idx).
template<>
apDATA_TRACKER::RECORD &dsVECTOR<apDATA_TRACKER::RECORD, 8>::operator[](int idx)
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
