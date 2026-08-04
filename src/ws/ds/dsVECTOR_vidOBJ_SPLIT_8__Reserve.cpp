#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"

// dsVECTOR<vidOBJ_SPLIT,8>::Reserve @ 0x82C87358
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<vidOBJ_SPLIT, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
