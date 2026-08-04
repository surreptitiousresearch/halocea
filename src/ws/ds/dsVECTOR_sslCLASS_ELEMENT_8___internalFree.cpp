#include "dsVECTOR.h"
#include "../ssl/sslCLASS_ELEMENT.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslCLASS_ELEMENT,8>::_internalFree @ 0x82A37AE8
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<sslCLASS_ELEMENT, 8>::_internalFree(void *p)
{
    dlFree(p);
}
