#include "dsVECTOR.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

// dsVECTOR<dscBRAND*,8>::Clear @ 0x82663230
// Destroy every live element (trivial no-op for pointer elements) and reset the count.
template<>
void dsVECTOR<dscBRAND *, 8>::Clear()
{
    int nElem = this->nElem;
    for (int i = 0; i < nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    this->nElem = 0;
}
