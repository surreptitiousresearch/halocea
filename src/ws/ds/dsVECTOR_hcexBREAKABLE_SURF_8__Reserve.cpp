#include "dsVECTOR.h"

struct hcexBREAKABLE_SURF; // boundary

// dsVECTOR<hcexBREAKABLE_SURF,8>::Reserve @ 0x823B20B0
// Grow the backing storage to hold at least `size` elements; a no-op when capacity already
// suffices.
template<>
void dsVECTOR<hcexBREAKABLE_SURF, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
