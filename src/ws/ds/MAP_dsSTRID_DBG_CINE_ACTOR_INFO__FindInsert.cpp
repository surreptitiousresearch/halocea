#include "MAP.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "dsSTRID.h"

// ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,...>::FindInsert(const K*, const V*) @ 0x823B9FD8
// Find-or-insert: probe Find(key) first; on a hit, return that iterator unchanged. On a miss,
// InsertNew(key, *value) and return its iterator instead.
template<>
ds::impl::MAP_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::FindInsert(const dsSTRID *key, const DBG_CINE_ACTOR_INFO *value)
{
    ds::impl::MAP_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> found = Find(key);
    if (found.item)
        return found;
    return InsertNew(key, value);
}
