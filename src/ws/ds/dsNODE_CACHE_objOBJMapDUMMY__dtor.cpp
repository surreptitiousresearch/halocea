#include "dsNODE_CACHE.h"
#include "dsObjOBJVecMapDUMMY.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// dsNODE_CACHE<dsObjOBJVecMapDUMMY>::~dsNODE_CACHE() @ 0x82689E08
// Term() frees every owned node block and clears the free list; then free nodeList's own backing
// array (the block-pointer array itself, already emptied by Term's Realloc(0) — freed again here
// matching the disassembly's explicit dlFree(nodeList.pData), a defensive no-op once Term already
// nulled it via Realloc(0)).
template<>
dsNODE_CACHE<dsObjOBJVecMapDUMMY>::~dsNODE_CACHE()
{
    this->Term();
    dlFree(this->nodeList.pData);
}
