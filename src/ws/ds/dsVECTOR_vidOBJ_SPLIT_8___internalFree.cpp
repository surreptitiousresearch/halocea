#include "dsVECTOR.h"
#include "../vid/vidOBJ_SPLIT.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<vidOBJ_SPLIT,8>::_internalFree @ 0x82C86598
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<vidOBJ_SPLIT, 8>::_internalFree(void *p)
{
    dlFree(p);
}
