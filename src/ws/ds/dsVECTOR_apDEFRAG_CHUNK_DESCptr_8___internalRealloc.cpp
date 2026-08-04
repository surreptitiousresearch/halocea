#include "dsVECTOR.h"

struct apDEFRAG_CHUNK_DESC; // boundary — ws-engine ap: defrag pool chunk descriptor (element is a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<apDEFRAG_CHUNK_DESC*,8>::_internalRealloc @ 0x823F0818
// Resize the backing store to hold exactly `size` elements via a single dlRealloc (pointer
// elements are trivially relocatable). Element size 4 bytes.
template<>
void dsVECTOR<apDEFRAG_CHUNK_DESC *, 8>::_internalRealloc(int size)
{
    this->pData = (apDEFRAG_CHUNK_DESC **)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
