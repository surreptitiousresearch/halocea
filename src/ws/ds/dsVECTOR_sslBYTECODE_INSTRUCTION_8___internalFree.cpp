#include "dsVECTOR.h"
#include "../ssl/sslBYTECODE_INSTRUCTION.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<sslBYTECODE_INSTRUCTION,8>::_internalFree @ 0x825B6460
// Release the backing element storage via the debug allocator.
template<>
void dsVECTOR<sslBYTECODE_INSTRUCTION, 8>::_internalFree(void *p)
{
    dlFree(p);
}
