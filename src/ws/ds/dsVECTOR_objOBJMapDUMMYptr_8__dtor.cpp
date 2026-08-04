#include "dsVECTOR.h"
#include "dsObjOBJVecMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsVECTOR<...objOBJ-map DUMMY*,8>::~dsVECTOR @ 0x82684370
// Pointer elements are trivially destructible, so the destructor collapses to freeing the backing
// element storage.
template<>
dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::~dsVECTOR()
{
    dlFree(this->pData);
}
