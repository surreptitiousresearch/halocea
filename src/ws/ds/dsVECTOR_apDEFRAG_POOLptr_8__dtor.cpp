#include "dsVECTOR.h"

struct apDEFRAG_POOL; // boundary — ws-engine ap: defragmentable memory pool (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apDEFRAG_POOL*,8>::~dsVECTOR @ 0x827B2B10
// Pointer elements are not owned by the vector; just free the backing element storage.
template<>
dsVECTOR<apDEFRAG_POOL *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
