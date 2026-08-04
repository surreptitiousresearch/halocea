#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::Clear @ 0x82556560
// Destroy every live element and reset the count. The element is a raw pointer (trivially
// destructible), so the per-element destructor loop is empty and collapses to nElem=0.
template<>
void dsVECTOR<dsMsgAddrMapITEM *, 8>::Clear()
{
    this->nElem = 0;
}
