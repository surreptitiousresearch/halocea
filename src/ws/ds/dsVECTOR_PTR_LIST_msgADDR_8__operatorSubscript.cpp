#include "dsVECTOR.h"
#include "PTR_LIST.h"
#include "ds_assert_boundary.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::operator[] @ 0x825598C8
// Bounds-checked element access; returns a reference to the PTR_LIST head record at `idx`. Only
// the non-const overload was found for this instantiation (no `QBA`-mangled sibling in the DB).
template<>
ds::PTR_LIST<msgADDR> &dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::operator[](int idx)
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
