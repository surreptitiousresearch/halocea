#include "dsVECTOR.h"

struct apDEFRAG_CHUNK_DESC; // boundary — ws-engine ap: defrag pool chunk descriptor (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apDEFRAG_CHUNK_DESC*,8>::_internalFree @ 0x823F0790
// Free a backing-storage block (protected helper; pointer elements need no per-element teardown).
template<>
void dsVECTOR<apDEFRAG_CHUNK_DESC *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
