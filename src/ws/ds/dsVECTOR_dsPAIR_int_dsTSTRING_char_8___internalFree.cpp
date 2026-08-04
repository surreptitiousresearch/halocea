#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::_internalFree @ 0x825C9E90
template<>
void dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::_internalFree(void *p)
{
    dlFree(p);
}
