#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct jbmJOB; // boundary — job-manager work item (jbm subsystem); only a pointer is handled

// dsVECTOR<jbmJOB*,8>::Front @ 0x826EBF80
// Return a reference to the first live job pointer; assert the vector is non-empty first.
template<>
jbmJOB *&dsVECTOR<jbmJOB *, 8>::Front()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            507,
            "nElem",
            this->nElem);
    return this->pData[0];
}
