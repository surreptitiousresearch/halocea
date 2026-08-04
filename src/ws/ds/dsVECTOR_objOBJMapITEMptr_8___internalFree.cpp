#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::_internalFree @ 0x826811E0
template<>
void dsVECTOR<dsObjOBJVecMapITEM *, 8>::_internalFree(void *p)
{
    dlFree(p);
}
