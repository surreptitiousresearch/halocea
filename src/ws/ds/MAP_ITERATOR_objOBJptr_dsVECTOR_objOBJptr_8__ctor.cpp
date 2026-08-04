#include "MAP.h"
struct objOBJ; // fwd (pointer element only)

// ds::impl::MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::MAP_ITERATOR() @ 0x82684420
// Default ctor: null/zero every field (done iterator).
template<>
ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP_ITERATOR()
{
    this->container = nullptr;
    this->item = nullptr;
    this->index = 0;
}
