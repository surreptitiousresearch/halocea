#include "dsVECTOR.h"
#include "dsPARAM.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPARAM,8>::_internalFree @ 0x823CFB50
template<>
void dsVECTOR<dsPARAM, 8>::_internalFree(void *p)
{
    dlFree(p);
}
