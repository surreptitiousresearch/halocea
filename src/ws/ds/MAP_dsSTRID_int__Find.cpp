#include "MAP.h"
#include "dsSTRID.h"

// ds::MAP<dsSTRID,int,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Find(const K*) @ 0x82766A20
// (apCONFIG::exportMap — config key -> dsSTATE_MGR state id). Same bucket-probe/chain-walk shape
// as the DBG_CINE_ACTOR_INFO instantiation of this map template (see
// MAP_dsSTRID_DBG_CINE_ACTOR_INFO__Find.cpp): locate `*key`'s bucket via GetHash<dsSTRID> %
// data.nElem, then walk the chain comparing interned ids (pointer equality) directly.
template<>
ds::impl::MAP_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Find(const dsSTRID *key)
{
    ds::impl::MAP_ITERATOR<dsSTRID, int, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> result;

    int nElem = data.nElem;
    if (nElem) {
        int bucketIndex = GetHash<dsSTRID>(key) % nElem;
        ITEM *node = data[bucketIndex];
        while (node) {
            if (key->id == node->key.id) {
                result.container = this;
                result.item = node;
                result.index = bucketIndex;
                return result;
            }
            node = node->next;
        }
    }

    result.container = nullptr;
    result.item = nullptr;
    result.index = 0;
    return result;
}
