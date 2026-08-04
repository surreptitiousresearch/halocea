#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::End @ 0x82C182C0
// Pointer one-past the last live element.
template<>
const dsSTRID *dsVECTOR<dsSTRID, 8>::End() const
{
    return &this->pData[this->nElem];
}
