#include "dsVECTOR.h"
#include "../ap/apDATA_TRACKER.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apDATA_TRACKER::RECORD,8>::_internalFree @ 0x82A242D8
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<apDATA_TRACKER::RECORD, 8>::_internalFree(void *p)
{
    dlFree(p);
}
