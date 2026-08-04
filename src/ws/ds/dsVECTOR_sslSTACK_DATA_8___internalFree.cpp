#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslSTACK_DATA,8>::_internalFree @ 0x825C9DF0
template<>
void dsVECTOR<sslSTACK_DATA, 8>::_internalFree(void *p)
{
    dlFree(p);
}
