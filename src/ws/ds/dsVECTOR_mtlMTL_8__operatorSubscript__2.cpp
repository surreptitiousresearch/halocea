#include "dsVECTOR.h"
#include "../mtl/mtlMTL.h"
#include "ds_assert_boundary.h"

// dsVECTOR<mtlMTL,8>::operator[](int) const @ 0x82C2B0E8
// Const reference to the element at `index`. Asserts 0 <= index < nElem.
// NOTE: same phantom-param artifact as the non-const overload (dsVECTOR_mtlMTL_8__operatorSubscript.cpp)
// — only `index` is a real parameter.
template<>
const mtlMTL &dsVECTOR<mtlMTL, 8>::operator[](int index) const
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", index,
            "nElem", this->nElem);
    return this->pData[index];
}
