#include "dsVECTOR.h"
#include "../ssl/sslCLASS_REF.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslCLASS_REF,8>::operator[] @ 0x825CCF38
template<>
sslCLASS_REF &dsVECTOR<sslCLASS_REF, 8>::operator[](int index)
{
    if (!IGNORE_STRONG_ASSERT && (index < 0 || index >= this->nElem))
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            567,
            "idx", index, "nElem", this->nElem);
    return this->pData[index];
}
