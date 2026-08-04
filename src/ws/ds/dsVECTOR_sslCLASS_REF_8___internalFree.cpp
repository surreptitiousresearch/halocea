#include "dsVECTOR.h"
#include "../ssl/sslCLASS_REF.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslCLASS_REF,8>::_internalFree @ 0x825C9EC8
template<>
void dsVECTOR<sslCLASS_REF, 8>::_internalFree(void *p)
{
    dlFree(p);
}
