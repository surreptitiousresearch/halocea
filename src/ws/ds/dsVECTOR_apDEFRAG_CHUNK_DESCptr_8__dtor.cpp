#include "dsVECTOR.h"

struct apDEFRAG_CHUNK_DESC; // boundary — ws-engine ap: defrag pool chunk descriptor (element is a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apDEFRAG_CHUNK_DESC*,8>::~dsVECTOR @ 0x823F1148
// Pointer elements are not owned by the vector; just free the backing element storage.
template<>
dsVECTOR<apDEFRAG_CHUNK_DESC *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
