#include "dsVECTOR.h"
#include "../ssl/sslCLASS_MAP_ELEM.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslCLASS_MAP_ELEM,8>::_internalFree @ 0x825C9E88
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<sslCLASS_MAP_ELEM, 8>::_internalFree(void *p)
{
    dlFree(p);
}
