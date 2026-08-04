#include "MAP.h"
struct objOBJ; // boundary — map key type, used pointer-only

// ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Begin() @ 0x82686130
// Scan forward from bucket 0 for the first non-null bucket head; wraps the resulting
// MAP_ITERATOR (or the empty/done iterator, if no bucket has an entry) in a BEGIN_HELPER for
// MAP_ITERATOR's ctor(const BEGIN_HELPER&) to unwrap.
template<>
ds::impl::BEGIN_HELPER<ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> >
ds::MAP<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Begin()
{
    typedef ds::impl::MAP_ITERATOR<objOBJ *, dsVECTOR<objOBJ *, 8>, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> ResultIter;

    int bucketCount = this->data.nElem;
    int bucket = 0;
    while (bucket < bucketCount && !this->data[bucket])
        ++bucket;

    ds::impl::BEGIN_HELPER<ResultIter> helper;
    if (bucket < bucketCount) {
        helper.iter.container = this;
        helper.iter.item = this->data[bucket];
        helper.iter.index = bucket;
    } else {
        helper.iter.container = nullptr;
        helper.iter.item = nullptr;
        helper.iter.index = 0;
    }
    return helper;
}
