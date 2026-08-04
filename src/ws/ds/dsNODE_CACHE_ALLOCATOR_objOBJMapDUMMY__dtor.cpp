#include "dsNODE_CACHE_ALLOCATOR.h"
#include "dsObjOBJVecMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::~dsNODE_CACHE_ALLOCATOR() @ 0x8268A9B0
// Term() + destroy nodeList (mirrors dsNODE_CACHE's own dtor; the compiler emitted this as a
// separate copy for the allocator specialization instead of reusing the base dtor).
template<>
dsNODE_CACHE_ALLOCATOR<dsObjOBJVecMapDUMMY>::~dsNODE_CACHE_ALLOCATOR()
{
    this->Term();
    dlFree(this->nodeList.pData);
}
