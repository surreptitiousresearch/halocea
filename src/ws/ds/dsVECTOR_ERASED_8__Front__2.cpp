#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

// boundary — dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,int,dsSTR_HASH,dsSTR_CMP>::LINE_ERASER>::ERASED,
// a deferred-erase bookkeeping record for the int-valued string map. Opaque here.
#include "dsStrIntMapERASED.h"

// dsVECTOR<...int-map ERASED,8>::Front @ 0x82616AE8
// Return a reference to the first live element; assert the vector is non-empty first.
template<>
dsStrIntMapERASED &dsVECTOR<dsStrIntMapERASED, 8>::Front()
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
