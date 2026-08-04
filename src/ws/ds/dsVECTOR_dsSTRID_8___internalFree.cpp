#include "dsVECTOR.h"
#include "dsSTRID.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsSTRID,8>::_internalFree @ 0x823C1DE8
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<dsSTRID, 8>::_internalFree(void *p)
{
    dlFree(p);
}
