#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::Begin (const) @ 0x827AE708
// Const pointer to the first element of the backing storage.
template<>
const dsDATA *dsVECTOR<dsDATA, 8>::Begin() const
{
    return this->pData;
}
