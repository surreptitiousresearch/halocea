#include "dsVECTOR.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

// dsVECTOR<propBASE*,8>::Clear @ 0x826C1E10
// Destroy every live element (trivial no-op for pointer elements) and reset the count.
template<>
void dsVECTOR<propBASE *, 8>::Clear()
{
    int nElem = this->nElem;
    for (int i = 0; i < nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    this->nElem = 0;
}
