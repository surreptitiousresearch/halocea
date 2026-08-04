#include "dsVECTOR.h"
#include "ds_assert_boundary.h"

struct cdtBONE; // boundary — ws-engine cdt: collision-detection skeleton bone (element is a pointer)

// dsVECTOR<cdtBONE*,8>::Back @ 0x828BBBE0
// Reference to the last live element pointer (index nElem-1); assert non-empty first.
template<>
cdtBONE *&dsVECTOR<cdtBONE *, 8>::Back()
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
