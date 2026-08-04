#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<...msgADDR-map ITEM*,8>::_internalFree @ 0x825565B0
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<dsMsgAddrMapITEM *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
