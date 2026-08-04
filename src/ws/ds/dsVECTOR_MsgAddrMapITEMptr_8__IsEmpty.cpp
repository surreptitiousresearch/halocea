#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::IsEmpty @ 0x825562C8
// True when the vector has no live elements.
template<>
bool dsVECTOR<dsMsgAddrMapITEM *, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
