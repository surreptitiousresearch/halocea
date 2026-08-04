#include "dsVECTOR.h"
#include "dsSTRID.h"
#include "ds_assert_boundary.h"

// dsVECTOR<dsSTRID,8>::Front @ 0x82F17470
// Reference to the first live element (asserts nElem>0).
template<>
dsSTRID &dsVECTOR<dsSTRID, 8>::Front()
{
    if (!IGNORE_STRONG_ASSERT && this->nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int>(
            "nElem > 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            507,
            "nElem",
            this->nElem);
    return *this->pData;
}
