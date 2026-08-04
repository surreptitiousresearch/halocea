#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

class dbgVAR; // boundary — dbg subsystem debug variable record (element is a pointer)

// dsVECTOR<dbgVAR*,8>::~dsVECTOR @ 0x8255DEC8
// Pointer elements are trivially destructible, so the destructor collapses to freeing the
// backing element storage.
template<>
dsVECTOR<dbgVAR *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
