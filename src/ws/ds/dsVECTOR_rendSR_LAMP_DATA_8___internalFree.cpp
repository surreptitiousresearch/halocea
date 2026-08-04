#include "dsVECTOR.h"
#include "../rend/rendSR_LAMP_DATA.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<rendSR_LAMP_DATA,8>::_internalFree @ 0x8252C048
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<rendSR_LAMP_DATA, 8>::_internalFree(void *p)
{
    dlFree(p);
}
