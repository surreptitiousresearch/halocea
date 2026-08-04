#include "dsVECTOR.h"
#include "dsAFFIX_BUILDER.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM,8>::Back @ 0x82A76710
// Reference to the last live affix parameter (index nElem-1); assert non-empty first.
template<>
dsAFFIX_BUILDER::AFF_PARAM &dsVECTOR<dsAFFIX_BUILDER::AFF_PARAM, 8>::Back()
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
