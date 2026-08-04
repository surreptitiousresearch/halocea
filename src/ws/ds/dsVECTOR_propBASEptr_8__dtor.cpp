#include "dsVECTOR.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<propBASE*,8>::~dsVECTOR @ 0x826C2630
// Pointer elements are not owned (the per-element destructor loop body is a no-op); free the
// backing element storage.
template<>
dsVECTOR<propBASE *, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    dlFree(this->pData);
}
