#include "dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

struct dsObjOBJVecMapITEM; // boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,...>::ITEM (opaque; pointer element)

// dsVECTOR<...objOBJ-map ITEM*,8>::~dsVECTOR @ 0x82684368
// Pointer elements are trivially destructible, so the destructor collapses to freeing the backing
// element storage.
template<>
dsVECTOR<dsObjOBJVecMapITEM *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
