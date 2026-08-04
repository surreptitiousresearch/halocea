#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../scn/scnINST_CREATE_DATA.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::_internalFree @ 0x823CFC60
template<>
void dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::_internalFree(void *p)
{
    dlFree(p);
}
