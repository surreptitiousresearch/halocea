#include "dsVECTOR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<dsTSTRING<char>,8>::_internalFree @ 0x823B3178
template<>
void dsVECTOR<dsTSTRING<char>, 8>::_internalFree(void *p)
{
    dlFree(p);
}
