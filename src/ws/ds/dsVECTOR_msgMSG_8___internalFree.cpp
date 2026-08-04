#include "dsVECTOR.h"
#include "../msg/msgMSG.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<msgMSG,8>::_internalFree @ 0x82556450
template<>
void dsVECTOR<msgMSG, 8>::_internalFree(void *p)
{
    dlFree(p);
}
