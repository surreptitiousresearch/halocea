#include "dsVECTOR.h"

#include "dsMsgAddrMapDUMMY.h"

// dsVECTOR<...msgADDR-map DUMMY*,8>::Compact @ 0x8255A278
// Shrink the backing store to exactly fit the live elements.
template<>
void dsVECTOR<dsMsgAddrMapDUMMY *, 8>::Compact()
{
    this->Realloc(this->nElem);
}
