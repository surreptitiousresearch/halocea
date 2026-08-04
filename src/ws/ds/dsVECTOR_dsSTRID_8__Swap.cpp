#include "dsVECTOR.h"
#include "dsSTRID.h"

// dsVECTOR<dsSTRID,8>::Swap @ 0x82C18140
// Exchange the three backing-storage fields (pData/nElem/allocated) with `other`.
template<>
void dsVECTOR<dsSTRID, 8>::Swap(dsVECTOR<dsSTRID, 8> &other)
{
    dsSTRID *pData = this->pData;
    this->pData = other.pData;
    other.pData = pData;

    int nElem = this->nElem;
    this->nElem = other.nElem;
    other.nElem = nElem;

    int allocated = this->allocated;
    this->allocated = other.allocated;
    other.allocated = allocated;
}
