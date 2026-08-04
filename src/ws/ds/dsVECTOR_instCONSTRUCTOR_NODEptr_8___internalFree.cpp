#include "dsVECTOR.h"

struct instCONSTRUCTOR_NODE; // boundary — ws-engine prop: instance-constructor graph node (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<instCONSTRUCTOR_NODE*,8>::_internalFree @ 0x826DCFC8
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<instCONSTRUCTOR_NODE *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
