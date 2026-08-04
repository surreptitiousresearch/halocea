#include "dsVECTOR.h"

struct hcexBREAKABLE_SURF; // boundary

// dsVECTOR<hcexBREAKABLE_SURF,8>::Free @ 0x823B2448
// Release all backing storage (reallocate down to zero capacity).
template<>
void dsVECTOR<hcexBREAKABLE_SURF, 8>::Free()
{
    this->Realloc(0);
}
