#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::_internalFree @ 0x823CFAD8
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::_internalFree(void *p)
{
    dlFree(p);
}
