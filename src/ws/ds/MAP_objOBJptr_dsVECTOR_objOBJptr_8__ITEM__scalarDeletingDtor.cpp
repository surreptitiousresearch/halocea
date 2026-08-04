#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::ITEM::
// `scalar deleting destructor' @ 0x82684BD0
// Compiler-generated deleting-destructor entry point: runs ~ITEM(), then conditionally calls
// operator delete(this) when bit 0 of the flag argument is set.
template<>
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM *
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM::scalarDeletingDtor(
    unsigned char deleteFlags)
{
    this->~ITEM();
    if (deleteFlags & 1)
        operator delete(this);
    return this;
}
