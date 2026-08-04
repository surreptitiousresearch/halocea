#include "dsVECTOR.h"

struct apCOUNTER; // boundary — ap performance counter; element is a pointer.

// dsVECTOR<apCOUNTER*,8>::Length @ 0x82714138
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<apCOUNTER *, 8>::Length() const
{
    return this->nElem;
}
