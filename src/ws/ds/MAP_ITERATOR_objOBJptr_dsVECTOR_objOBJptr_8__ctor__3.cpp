#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::impl::MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::MAP_ITERATOR(const BEGIN_HELPER&)
// @ 0x82680B10
// Unwrap the BEGIN_HELPER Begin() returns (bytewise copy of its wrapped `iter`).
template<>
ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP_ITERATOR(
    const ds::impl::BEGIN_HELPER<ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> > &helper)
{
    *this = helper.iter;
}
