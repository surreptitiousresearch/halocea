#include "dsVECTOR.h"

struct apDEFRAG_POOL; // boundary — ws-engine ap: defragmentable memory pool (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apDEFRAG_POOL*,8>::_internalFree @ 0x827B2788
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<apDEFRAG_POOL *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
