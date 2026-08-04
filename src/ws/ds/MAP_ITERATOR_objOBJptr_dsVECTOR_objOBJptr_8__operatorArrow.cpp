#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::impl::MAP_ITERATOR<objOBJ*,dsVECTOR<objOBJ*,8>,...>::operator->() @ 0x826842D0
// Thunk: forwards to the base MAP_CONST_ITERATOR::operator-> (same STRONG_ASSERT + matched-node
// value pointer), reinterpreted mutable.
template<>
dsVECTOR<objOBJ *, 8> *ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::operator->() const
{
    return const_cast<dsVECTOR<objOBJ *, 8> *>(
        ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::operator->());
}
