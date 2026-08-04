#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE; // boundary — ws-engine prop: instance-constructor graph node (element is a pointer)

// dsVECTOR<instCONSTRUCTOR_NODE*,8>::Clear @ 0x826DCF10
// Destroy every live element (trivial no-op for pointer elements) and reset the count.
template<>
void dsVECTOR<instCONSTRUCTOR_NODE *, 8>::Clear()
{
    int nElem = this->nElem;
    for (int i = 0; i < nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    this->nElem = 0;
}
