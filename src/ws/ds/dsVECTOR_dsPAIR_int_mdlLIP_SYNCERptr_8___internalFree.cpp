#include "dsVECTOR.h"
#include "dsPAIR.h"

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::_internalFree @ 0x823CFCB0
template<>
void dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::_internalFree(void *p)
{
    dlFree(p);
}
