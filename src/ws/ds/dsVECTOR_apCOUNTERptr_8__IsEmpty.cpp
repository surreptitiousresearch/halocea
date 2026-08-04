#include "dsVECTOR.h"

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::IsEmpty @ 0x82714128
// True when there are no live elements.
template<>
bool dsVECTOR<apCOUNTER *, 8>::IsEmpty() const
{
    return this->nElem == 0;
}
