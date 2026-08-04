#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::operator[] (const) @ 0x82A76440
// Bounds-checked read-only access; returns a const reference to the idx-th affix parameter.
template<>
const dsAFFIX_BUILDER::AFF_PARAM &dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::operator[](int idx) const
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            557,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
