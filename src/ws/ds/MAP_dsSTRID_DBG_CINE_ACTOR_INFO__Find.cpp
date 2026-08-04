#include "MAP.h"
#include "../dbg/DBG_CINE_ACTOR_INFO.h"
#include "dsSTRID.h"

// ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Find(const K*)
// @ 0x823B65F0
// Locate `*key`'s bucket via GetHash<dsSTRID> % data.nElem, then walk the chain comparing interned
// ids directly (dsSTRID equality is pointer equality: dsSTRID::id is the pooled/interned pointer).
// Returns a MAP_ITERATOR whose `item` is null on a miss. DEVIATION: the decompile's bucket-index
// computation is wrapped in bogus-looking `__twllei`/`__twlgei` trap-word-compare intrinsics and a
// redundant re-load of `dsVECTOR::operator[]` for the truthiness check — both are PowerPC
// bounds-check/branch artifacts of the inlined `operator[]`, not real control flow; collapsed here
// to the single `hash % nElem` bucket lookup the surrounding logic actually performs.
template<>
ds::impl::MAP_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>
ds::MAP<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Find(const dsSTRID *key)
{
    ds::impl::MAP_ITERATOR<dsSTRID, DBG_CINE_ACTOR_INFO, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> result;

    int nElem = this->data.nElem;
    if (nElem) {
        int bucketIndex = this->GetHash<dsSTRID>(key) % nElem;
        ITEM *node = this->data[bucketIndex];
        if (node) {
            while (key->id != node->key.id) {
                node = node->next;
                if (!node) {
                    result.container = nullptr;
                    result.item = nullptr;
                    result.index = 0;
                    return result;
                }
            }
            result.container = this;
            result.item = node;
            result.index = bucketIndex;
            return result;
        }
    }

    result.container = nullptr;
    result.item = nullptr;
    result.index = 0;
    return result;
}
