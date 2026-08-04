#include "dsVECTOR.h"

struct camCAMERA; // boundary — ws-engine cam: render camera (element is a pointer)

// dsVECTOR<camCAMERA*,8>::Length @ 0x82703B30
// Return the live element count (nElem @ 0x4).
template<>
int dsVECTOR<camCAMERA *, 8>::Length() const
{
    return this->nElem;
}
