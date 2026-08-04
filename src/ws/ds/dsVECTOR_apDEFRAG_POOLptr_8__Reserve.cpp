#include "dsVECTOR.h"

struct apDEFRAG_POOL; // boundary — ws-engine ap: defragmentable memory pool (element is a pointer)

// dsVECTOR<apDEFRAG_POOL*,8>::Reserve @ 0x823F1F78
// Grow the backing store to at least `size` elements when it isn't already that large; a no-op
// when capacity is already sufficient.
template<>
void dsVECTOR<apDEFRAG_POOL *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
