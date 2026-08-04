#include "dsVECTOR.h"

#include "dsMsgAddrMapDUMMY.h"

// dsVECTOR<...msgADDR-map DUMMY*,8>::Length @ 0x825562F0
// Number of live elements.
template<>
int dsVECTOR<dsMsgAddrMapDUMMY *, 8>::Length() const
{
    return this->nElem;
}
