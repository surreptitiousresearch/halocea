#include "MAP.h"
struct objOBJ; // fwd (pointer element only)

// ds::impl::MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::MAP_CONST_ITERATOR(MAP&,ITEM&,int)
// @ 0x826810B8
// Position at `item` (`container`/bucket `index` supplied by the caller, e.g. MAP::Find/Begin).
template<>
ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::MAP_CONST_ITERATOR(
    ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> *container_,
    ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::ITEM *item_,
    int index_)
    : container(container_), item(item_), index(index_)
{
}
