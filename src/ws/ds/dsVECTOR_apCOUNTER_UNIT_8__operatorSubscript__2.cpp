#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"
#include "ds_assert_boundary.h"

// dsVECTOR<apCOUNTER_UNIT,8>::operator[] const @ 0x8250BE18
template<>
const apCOUNTER_UNIT &dsVECTOR<apCOUNTER_UNIT, 8>::operator[](int index) const
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", index, "nElem", this->nElem);
    return this->pData[index];
}
