#include "dsVECTOR.h"
#include "dsStrIntMapERASED.h"
#include "ds_assert_boundary.h"

// dsVECTOR<...int-map ERASED,8>::Back @ 0x82616B58
// Reference to the last live deferred-erase record (index nElem-1); assert non-empty first.
// Element is dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,int,...>::LINE_ERASER>::ERASED.
template<>
dsStrIntMapERASED &dsVECTOR<dsStrIntMapERASED, 8>::Back()
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
