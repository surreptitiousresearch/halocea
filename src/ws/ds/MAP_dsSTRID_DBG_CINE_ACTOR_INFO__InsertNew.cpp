#include "MAP.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "dsSTRID.h"

// ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...>::InsertNew(const K*, const V*) @ 0x823B9EFC
// Unconditional insert (caller has already verified the key is absent): lazily HashResize(17) if
// the table has no buckets yet, compute the new node's bucket via GetHash<dsSTRID>(key) %
// data.nElem, bump `count`, prepend the node onto that bucket's chain via the 3-arg Insert
// helper, then return a MAP_ITERATOR positioned at the just-linked node (re-resolved by index
// AFTER the insert, matching FindInsert's re-probe convention on a resize/insert).
template<>
ds::impl::MAP_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::InsertNew(const dsSTRID *key, const DBG_CINE_ACTOR_INFO *value)
{
    if (!data.nElem)
        data.Resize(17, nullptr);

    int nElem = data.nElem;
    unsigned int hash = GetHash<dsSTRID>(key);
    ++count;
    int bucketIndex = hash % nElem;

    Insert(&data[bucketIndex], *key, *value);

    ds::impl::MAP_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> result;
    result.container = this;
    result.index = bucketIndex;
    result.item = data[bucketIndex];
    return result;
}
