#include "MAP.h"
struct msgADDR; // fwd (pointer element only)

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Find @ 0x82559468
// Look up `*key`; walks the chain at bucket (GetHash(key) % data.nElem) comparing keys with `==`.
// Returns a null-item iterator on a miss or on an empty table (no buckets allocated yet).
template<>
ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Find(const int *key)
{
    if (!data.nElem)
        return ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>(0, 0, 0);

    unsigned int hash = GetHash<int>(key);
    int bucket = (int)(hash % (unsigned int)data.nElem);

    ITEM *node = data[bucket];
    while (node && *key != node->key)
        node = node->next;

    if (!node)
        return ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>(0, 0, 0);

    return ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>(this, node, bucket);
}
