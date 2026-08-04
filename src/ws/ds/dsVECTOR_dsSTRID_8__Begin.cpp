#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::Begin @ 0x82C182B8
// Pointer to the first element of the backing storage.
template<>
const dsSTRID *dsVECTOR<dsSTRID, 8>::Begin() const
{
    return this->pData;
}
