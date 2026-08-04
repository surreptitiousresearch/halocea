#include "MAP.h"
struct objOBJ; // fwd (pointer element only)

// ds::impl::MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::MAP_ITERATOR(MAP&,ITEM&,int) @ 0x82684438
// Position at `item` (`container`/bucket `index` supplied by the caller, e.g. MAP::Find/Begin).
template<>
ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP_ITERATOR(
    ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> *container_,
    ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM *item_,
    int index_)
{
    this->container = container_;
    this->item = item_;
    this->index = index_;
}
