#include "MAP.h"
struct objOBJ; // fwd (pointer element only)

// ds::impl::MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::MAP_CONST_ITERATOR() @ 0x82681080
// Default ctor: null/zero every field (done iterator).
template<>
ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP_CONST_ITERATOR()
    : container(nullptr), item(nullptr), index(0)
{
}
