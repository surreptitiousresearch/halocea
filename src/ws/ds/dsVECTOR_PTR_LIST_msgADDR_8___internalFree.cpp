#include "dsVECTOR.h"
#include "PTR_LIST.h"

struct msgADDR; // boundary — msg-system endpoint address (msg subsystem)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<ds::PTR_LIST<msgADDR>,8>::_internalFree @ 0x825566F0
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::_internalFree(void *p)
{
    dlFree(p);
}
