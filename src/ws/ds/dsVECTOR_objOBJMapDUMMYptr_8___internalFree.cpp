#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<...objOBJ-map DUMMY*,8>::_internalFree @ 0x82681218
template<>
void dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
