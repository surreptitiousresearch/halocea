#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

// dsVECTOR<camCAMERA*,8>::Clear @ 0x82703B50
// Destroy every live element (trivial no-op for pointer elements) and reset the count.
template<>
void dsVECTOR<camCAMERA *, 8>::Clear()
{
    int nElem = this->nElem;
    for (int i = 0; i < nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    this->nElem = 0;
}
