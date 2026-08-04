#include "dsVECTOR.h"

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM,
// a hash-map key/value entry record. Opaque here; the vector element is a pointer.
struct dsObjOBJVecMapITEM;

// dsVECTOR<...objOBJ-map ITEM*,8>::Length @ 0x82681070
template<>
int dsVECTOR<dsObjOBJVecMapITEM *, 8>::Length() const
{
    return this->nElem;
}
