#include "dsVECTOR.h"

#include "dsMsgAddrMapDUMMY.h"

// dsVECTOR<...msgADDR-map DUMMY*,8>::Clear @ 0x825565E8
// Destroy every live element and reset the count. The element is a raw pointer (trivially
// destructible), so the per-element destructor loop is empty and collapses to nElem=0.
template<>
void dsVECTOR<dsMsgAddrMapDUMMY *, 8>::Clear()
{
    this->nElem = 0;
}
