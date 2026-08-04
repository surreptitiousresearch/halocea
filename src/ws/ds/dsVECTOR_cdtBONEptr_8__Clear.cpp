#include "dsVECTOR.h"

struct cdtBONE; // boundary — ws-engine cdt: collision-detection skeleton bone (element is a pointer)

// dsVECTOR<cdtBONE*,8>::Clear @ 0x82B0B660
// Destroy every live element (trivial no-op for pointer elements) and reset the count.
template<>
void dsVECTOR<cdtBONE *, 8>::Clear()
{
    int nElem = this->nElem;
    for (int i = 0; i < nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    this->nElem = 0;
}
