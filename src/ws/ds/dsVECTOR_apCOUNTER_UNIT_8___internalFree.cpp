#include "dsVECTOR.h"
#include "../ap/apCOUNTER_UNIT.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<apCOUNTER_UNIT,8>::_internalFree @ 0x823CFB60
template<>
void dsVECTOR<apCOUNTER_UNIT, 8>::_internalFree(void *p)
{
    dlFree(p);
}
