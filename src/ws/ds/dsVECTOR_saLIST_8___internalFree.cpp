#include "dsVECTOR.h"
#include "../msg/saLIST.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<saLIST,8>::_internalFree @ 0x825567A8
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<saLIST, 8>::_internalFree(void *p)
{
    dlFree(p);
}
