#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

class dbgVAR; // boundary — dbg subsystem debug variable record (element is a pointer)

// dsVECTOR<dbgVAR*,8>::_internalFree @ 0x8255DBB0
// Free a raw storage block previously obtained from the debug allocator.
template<>
void dsVECTOR<dbgVAR *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
