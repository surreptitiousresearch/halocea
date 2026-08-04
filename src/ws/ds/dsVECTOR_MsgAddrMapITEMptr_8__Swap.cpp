#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::Swap @ 0x825564D8
// Exchange the backing store, element count, and capacity with another vector (three-field swap).
template<>
void dsVECTOR<dsMsgAddrMapITEM *, 8>::Swap(dsVECTOR<dsMsgAddrMapITEM *, 8> &other)
{
    dsMsgAddrMapITEM **pData = this->pData;
    this->pData = other.pData;
    other.pData = pData;

    int nElem = this->nElem;
    this->nElem = other.nElem;
    other.nElem = nElem;

    int allocated = this->allocated;
    this->allocated = other.allocated;
    other.allocated = allocated;
}
