#include "dsVECTOR.h"

#include "dsMsgAddrMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<...msgADDR-map DUMMY*,8>::_internalFree @ 0x82556638
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<dsMsgAddrMapDUMMY *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
