#include "dsVECTOR.h"
#include "../ssl/sslCLASS_NAME_TO_IDX.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslCLASS::NAME_TO_IDX,8>::_internalFree @ 0x82A37AE0
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<ssl_class::NAME_TO_IDX, 8>::_internalFree(void *p)
{
    dlFree(p);
}
