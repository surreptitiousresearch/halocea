#include "dsVECTOR.h"

struct inpDEVICE; // boundary — ws-engine inp: physical input device (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<inpDEVICE*,8>::_internalFree @ 0x823C1E20
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<inpDEVICE *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
