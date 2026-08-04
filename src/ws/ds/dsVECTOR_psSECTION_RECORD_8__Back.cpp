#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

#include "psSECTION_RECORD.h"

// dsVECTOR<psSECTION_RECORD,8>::Back @ 0x82518890
// Reference to the last live section record (index nElem-1); assert non-empty first.
template<>
psSECTION_RECORD &dsVECTOR<psSECTION_RECORD, 8>::Back()
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
