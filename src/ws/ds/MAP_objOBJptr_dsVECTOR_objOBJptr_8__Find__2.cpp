#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Find(const K*)
// const @ 0x82687C78
// Const overload: delegates to the mutable Find (a MAP_ITERATOR and MAP_CONST_ITERATOR share the
// exact same {container,item,index} layout, so the const-qualified result is just a bytewise copy
// of the mutable one's fields) — matches objDEPENDENCY_CACHE::AddSharingUsers's DependencyMapConstIter
// usage of this const overload on a `const objDEPENDENCY_CACHE*`.
template<>
ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Find(objOBJ *const *key) const
{
    ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> mutableResult =
        const_cast<ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> *>(this)->Find(key);

    ds::impl::MAP_CONST_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> result;
    result.container = mutableResult.container;
    result.item      = mutableResult.item;
    result.index      = mutableResult.index;
    return result;
}
