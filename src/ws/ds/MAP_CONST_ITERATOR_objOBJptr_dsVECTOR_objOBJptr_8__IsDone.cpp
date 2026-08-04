#include "MAP.h"
struct objOBJ; // fwd (pointer element only)

// ds::impl::MAP_CONST_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::IsDone() @ 0x82680BA0
template<>
bool ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::IsDone() const
{
    return this->item == nullptr;
}
