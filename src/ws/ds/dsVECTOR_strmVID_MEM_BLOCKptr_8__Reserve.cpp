#include "dsVECTOR.h"

class strmVID_MEM_BLOCK; // boundary — strm subsystem video-memory block (element is a pointer)

// dsVECTOR<strmVID_MEM_BLOCK*,8>::Reserve @ 0x8269A7A8
// Grow the backing store to at least `size` elements; a no-op if already large enough.
template<>
void dsVECTOR<strmVID_MEM_BLOCK *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
