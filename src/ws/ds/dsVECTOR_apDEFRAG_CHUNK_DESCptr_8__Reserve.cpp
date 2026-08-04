#include "dsVECTOR.h"

struct apDEFRAG_CHUNK_DESC; // boundary — ws-engine ap: defrag pool chunk descriptor (element is a pointer)

// dsVECTOR<apDEFRAG_CHUNK_DESC*,8>::Reserve @ 0x823F1F60
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<apDEFRAG_CHUNK_DESC *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
