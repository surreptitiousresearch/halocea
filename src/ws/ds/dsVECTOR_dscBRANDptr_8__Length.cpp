#include "dsVECTOR.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

// dsVECTOR<dscBRAND*,8>::Length @ 0x826426C0
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<dscBRAND *, 8>::Length() const
{
    return this->nElem;
}
