#include "dsVECTOR.h"
#include "dsStrStridMapERASED.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsERASE_MANAGER<dsLF_SORTED_MAP<const char*,dsSTRID,...>::LINE_ERASER>::ERASED,8>::Back
//   @ 0x8252A318
// Reference to the last live deferred-erase record (index nElem-1); assert non-empty first.
template<>
dsStrStridMapERASED &dsVECTOR<dsStrStridMapERASED, 8>::Back()
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
