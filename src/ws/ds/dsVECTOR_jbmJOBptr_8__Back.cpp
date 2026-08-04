#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct jbmJOB; // boundary — job-manager work item (jbm subsystem); only a pointer is handled

// dsVECTOR<jbmJOB*,8>::Back @ 0x8250BB50
// Reference to the last live job pointer (index nElem-1); assert non-empty first.
template<>
jbmJOB *&dsVECTOR<jbmJOB *, 8>::Back()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            527,
            "nElem",
            this->nElem);
    return this->pData[this->nElem - 1];
}
