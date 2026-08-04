#include "dsVECTOR.h"

struct strmVID_BUFFER; // boundary — strm subsystem video buffer (element is a pointer)

// dsVECTOR<strmVID_BUFFER*,8>::Reserve @ 0x8269A5F0
// Grow the backing store to at least `size` elements; a no-op if already large enough.
template<>
void dsVECTOR<strmVID_BUFFER *, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
