#include "dsVECTOR.h"

struct inpDEVICE; // boundary — ws-engine inp: physical input device (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<inpDEVICE*,8>::~dsVECTOR @ 0x823C2878
// Pointer elements are not owned (the per-element destructor loop body is a no-op); free the
// backing element storage.
template<>
dsVECTOR<inpDEVICE *, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        ; // trivially destructible pointer element; destructor loop emitted as a no-op
    dlFree(this->pData);
}
