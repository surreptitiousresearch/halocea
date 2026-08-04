#include "dsVECTOR.h"

struct inpDEVICE; // boundary — ws-engine inp: physical input device (element is a pointer)

// dsVECTOR<inpDEVICE*,8>::Clear @ 0x8263F718
// Destroy every live element (trivial no-op for pointer elements) and reset the count.
template<>
void dsVECTOR<inpDEVICE *, 8>::Clear()
{
    int nElem = this->nElem;
    for (int i = 0; i < nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    this->nElem = 0;
}
