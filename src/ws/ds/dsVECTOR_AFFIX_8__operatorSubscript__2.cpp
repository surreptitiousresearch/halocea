#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::operator[] (const) @ 0x82A76670
// Bounds-checked read-only access; returns a const reference to the idx-th affix.
template<>
const dsAFFIX_BUILDER::AFFIX &dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::operator[](int idx) const
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
