#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"
#include "ds_assert_boundary.h"

// dsVECTOR<sslSTACK_DATA,8>::Back const @ 0x82AF12C0
template<>
const sslSTACK_DATA &dsVECTOR<sslSTACK_DATA, 8>::Back() const
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            517,
            "nElem", this->nElem);
    return this->pData[this->nElem - 1];
}
