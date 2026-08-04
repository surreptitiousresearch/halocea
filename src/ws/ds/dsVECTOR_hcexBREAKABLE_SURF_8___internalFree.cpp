#include "dsVECTOR.h"

struct hcexBREAKABLE_SURF; // boundary

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<hcexBREAKABLE_SURF,8>::_internalFree @ 0x823B0C50
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<hcexBREAKABLE_SURF, 8>::_internalFree(void *p)
{
    dlFree(p);
}
