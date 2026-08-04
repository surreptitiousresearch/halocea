#include "dsVECTOR.h"

struct propBASE; // boundary — ws-engine prop: root property/component object (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<propBASE*,8>::_internalFree @ 0x826C1EE8
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<propBASE *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
