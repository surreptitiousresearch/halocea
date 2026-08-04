#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::Begin (non-const) @ 0x825233B0
// Pointer to the first element of the backing storage.
template<>
dsDATA *dsVECTOR<dsDATA, 8>::Begin()
{
    return this->pData;
}
