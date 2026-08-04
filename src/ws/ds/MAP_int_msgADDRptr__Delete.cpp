#include "MAP.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Delete @ 0x8255AD20
// Return `ptr` directly to the allocator's free list (tail-insert). Callers that have already
// unlinked the node from its bucket chain themselves use this instead of going through Erase.
template<>
void ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Delete(DUMMY *ptr)
{
    allocator.Delete(reinterpret_cast<dsMsgAddrMapDUMMY *>(ptr));
}
