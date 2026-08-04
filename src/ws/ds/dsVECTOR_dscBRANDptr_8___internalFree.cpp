#include "dsVECTOR.h"

struct dscBRAND; // boundary — ws-engine dsc: descriptor/type registry brand (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dscBRAND*,8>::_internalFree @ 0x826632C8
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<dscBRAND *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
