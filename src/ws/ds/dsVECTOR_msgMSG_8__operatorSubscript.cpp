#include "dsVECTOR.h"
#include "../msg/msgMSG.h"
#include "ds_assert_boundary.h"

// dsVECTOR<msgMSG,8>::operator[] @ 0x82AC4C78
template<>
msgMSG &dsVECTOR<msgMSG, 8>::operator[](int index)
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", index, "nElem", this->nElem);
    return this->pData[index];
}
