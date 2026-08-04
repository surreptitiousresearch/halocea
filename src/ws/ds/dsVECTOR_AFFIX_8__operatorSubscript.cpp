#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsAFFIX_BUILDER::AFFIX,8>::operator[] (non-const) @ 0x82A765D0
// Bounds-checked mutable access; returns a reference to the idx-th affix.
template<>
dsAFFIX_BUILDER::AFFIX &dsVECTOR<dsAFFIX_BUILDER::AFFIX, 8>::operator[](int idx)
{
    if (!IGNORE_STRONG_ASSERT && (idx < 0 || idx >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", idx,
            "nElem", this->nElem);
    return this->pData[idx];
}
