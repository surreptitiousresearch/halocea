#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::Length @ 0x825562E0
// Number of live elements.
template<>
int dsVECTOR<dsMsgAddrMapITEM *, 8>::Length() const
{
    return this->nElem;
}
