/* Flattened boundary definitions for the ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,
 * dsNODE_CACHE_ALLOCATOR> operations that DBG_CINE_INFO.h exposes over the opaque mapActors shim.
 * Each function is the compiler-generated thiscall form of the named ws-engine map method, spelled
 * as a free function taking the map by void* and returning the DBG_CINE_MAP_ITER_T position handle
 * by value. Bodies reproduce the decompiled template instantiations (addresses noted per function);
 * the deepest node-pool primitives (dsVECTOR::Resize, the 3-arg node-prepend Insert, Delete) are
 * left as boundary externs per the bounded-descent rule. */

#include "../headers/hcex/DBG_CINE_ACTOR_MAP_NODE.h"
#include <intrin.h> // __umulh (64x64->high64) for the hash reduction below

/* --- boundary externs (deepest node-pool ops; one bounded-descent level below these methods) --- */
/* ds::MAP<...>::Delete(this, node) — return one unlinked node to the allocator free list. */
extern void DBG_CINE_actor_map_Delete(void *map, DBG_CINE_ACTOR_MAP_ITEM *node);
/* dsVECTOR<ITEM*,8>::Resize(size, fill) — grow the bucket vector to `size` heads, each = *fill. */
extern void DBG_CINE_actor_bucket_Resize(DBG_CINE_ACTOR_BUCKET_VEC *vec, int size,
                                         DBG_CINE_ACTOR_MAP_ITEM *const *fill);
/* ds::MAP<...>::Insert(&bucketHead, key, value) @ 0x823B95A8 — allocate a node and prepend it. */
extern void DBG_CINE_actor_map_Insert(void *map, DBG_CINE_ACTOR_MAP_ITEM **bucketHead,
                                      const dsSTRID *key, const DBG_CINE_ACTOR_INFO *value);

/* ds::MAP<...>::Clear @ 0x823B8798 — return every live node to the free list, then reset the
 * bucket count and live count to 0 (bucket storage is kept, not freed). */
void DBG_CINE_INFO_mapActors_Clear(void *map_actors)
{
    DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)map_actors;

    for (int bucket = 0; bucket < map->data.nElem; ++bucket)
    {
        DBG_CINE_ACTOR_MAP_ITEM *node = map->data.pData[bucket];
        while (node)
        {
            DBG_CINE_ACTOR_MAP_ITEM *next = node->next;
            DBG_CINE_actor_map_Delete(map, node);
            node = next;
        }
    }

    /* The raw decompile has a second, empty `for` over data.nElem here — a dead artifact; dropped. */
    map->data.nElem = 0;
    map->count = 0;
}

/* ds::MAP<...>::GetHash<dsSTRID> @ 0x823B58E8 — mix key->id with the map's per-instance salts
 * (a,b) and reduce to [0, ds::BIG_PRIME) via a fixed multiply-high constant. Mirrors the reversed
 * MAP_dsSTRID_DBG_CINE_ACTOR_INFO__GetHash.cpp body. */
unsigned int DBG_CINE_INFO_mapActors_GetHash(void *map_actors, const dsSTRID *key)
{
    DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)map_actors;

    unsigned int mixed = (unsigned int)(uintptr_t)key->id * map->a + map->b;
    /* Raw decompile: dividend64 low dword = mixed, high dword = 0x55555560; the whole reduction is a
     * materialized `% ds::BIG_PRIME` (0x55555561 == 1431655777). */
    unsigned long long dividend64 = ((unsigned long long)0x55555560u << 32) | mixed;
    // __int128 is unavailable on MSVC/x64; the 128-bit product >>94 == high64(product) >> 30.
    unsigned int q = (unsigned int)(__umulh(dividend64, 0xBFFFFFE5C0000397ULL) >> 30);
    unsigned int val = mixed - 1431655777u * q;
    return val;
}

/* ds::MAP<...>::Find @ 0x823B65F0 — locate `*key`; the returned handle's `item` is null on a miss
 * (dsSTRID equality is interned-pointer equality). Mirrors MAP_dsSTRID_DBG_CINE_ACTOR_INFO__Find.cpp. */
DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_Find(void *map_actors, const dsSTRID *key)
{
    DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)map_actors;
    DBG_CINE_ACTOR_MAP_ITER result;

    int nElem = map->data.nElem;
    if (nElem)
    {
        int bucketIndex = (int)(DBG_CINE_INFO_mapActors_GetHash(map_actors, key) % (unsigned int)nElem);
        DBG_CINE_ACTOR_MAP_ITEM *node = map->data.pData[bucketIndex];
        while (node)
        {
            if (key->id == node->key.id)
            {
                result.container = map_actors;
                result.item = node;
                result.index = bucketIndex;
                return result;
            }
            node = node->next;
        }
    }

    result.container = 0;
    result.item = 0;
    result.index = 0;
    return result;
}

/* ds::MAP<...>::InsertNew @ 0x823B9EFC — unconditional insert (key known absent): lazily size the
 * bucket vector to 17 on first use, compute the bucket, bump `count`, prepend the node, then return
 * a handle positioned at the just-linked node. Mirrors MAP_dsSTRID_DBG_CINE_ACTOR_INFO__InsertNew.cpp. */
DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_InsertNew(void *map_actors, const dsSTRID *key,
                                                          const DBG_CINE_ACTOR_INFO *value)
{
    DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)map_actors;

    if (!map->data.nElem)
    {
        DBG_CINE_ACTOR_MAP_ITEM *emptyHead = 0;
        DBG_CINE_actor_bucket_Resize(&map->data, 17, &emptyHead);
    }

    int nElem = map->data.nElem;
    unsigned int hash = DBG_CINE_INFO_mapActors_GetHash(map_actors, key);
    ++map->count;
    int bucketIndex = (int)(hash % (unsigned int)nElem);

    DBG_CINE_actor_map_Insert(map_actors, &map->data.pData[bucketIndex], key, value);

    DBG_CINE_ACTOR_MAP_ITER result;
    result.container = map_actors;
    result.item = map->data.pData[bucketIndex]; /* re-resolved AFTER the insert (matches decompile) */
    result.index = bucketIndex;
    return result;
}

/* ds::MAP<...>::FindInsert @ 0x823B9FD8 — find-or-insert. Probe Find; on a hit return that handle,
 * otherwise InsertNew. Mirrors MAP_dsSTRID_DBG_CINE_ACTOR_INFO__FindInsert.cpp. */
DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_FindInsert(void *map_actors, const dsSTRID *key,
                                                           const DBG_CINE_ACTOR_INFO *value)
{
    DBG_CINE_ACTOR_MAP_ITER found = DBG_CINE_INFO_mapActors_Find(map_actors, key);
    if (found.item)
        return found;
    return DBG_CINE_INFO_mapActors_InsertNew(map_actors, key, value);
}

/* ds::MAP<...>::Begin @ 0x823B64D8 — position at the first live entry: scan forward for the first
 * non-empty bucket head, else return the done/null handle. (The raw decompile's BEGIN_HELPER
 * plumbing and the ds_vector bounds STRONG_ASSERT are elided; the field writes it performs are
 * container@a, item@b, index@count, exactly this handle.) */
DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_Begin(void *map_actors)
{
    DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)map_actors;
    DBG_CINE_ACTOR_MAP_ITER result;

    int bucket = 0;
    while (bucket < map->data.nElem && !map->data.pData[bucket])
        ++bucket;

    if (bucket < map->data.nElem)
    {
        result.container = map_actors;
        result.item = map->data.pData[bucket];
        result.index = bucket;
    }
    else
    {
        result.container = 0;
        result.item = 0;
        result.index = 0;
    }
    return result;
}

/* ds::impl::MAP_ITERATOR<...>::operator++ @ 0x823B7198 — advance to the next chained node, else the
 * next non-empty bucket; yields the done/null handle once the walk runs off the end. (The
 * already-done and empty-map STRONG_ASSERT-crash paths are elided; both fall through to the null
 * handle.) */
DBG_CINE_ACTOR_MAP_ITER DBG_CINE_INFO_mapActors_Next(DBG_CINE_ACTOR_MAP_ITER iter)
{
    DBG_CINE_ACTOR_MAP_ITEM *node = (DBG_CINE_ACTOR_MAP_ITEM *)iter.item;
    if (node)
    {
        if (node->next)
        {
            iter.item = node->next;
            return iter;
        }

        DBG_CINE_ACTOR_MAP_VIEW *map = (DBG_CINE_ACTOR_MAP_VIEW *)iter.container;
        if (map->data.nElem)
        {
            int index;
            do
                index = ++iter.index;
            while (index < map->data.nElem && !map->data.pData[index]);

            if (iter.index < map->data.nElem)
            {
                iter.item = map->data.pData[iter.index];
                return iter;
            }
        }

        iter.container = 0;
        iter.index = 0;
        iter.item = 0;
    }
    return iter;
}

/* ds::impl::MAP_ITERATOR<...>::operator-> @ 0x823B3A00 — thunk to the const-iterator operator->,
 * which STRONG_ASSERTs item != null and returns &item->value. */
DBG_CINE_ACTOR_INFO *DBG_CINE_INFO_mapActors_Deref(DBG_CINE_ACTOR_MAP_ITER iter)
{
    return &((DBG_CINE_ACTOR_MAP_ITEM *)iter.item)->value;
}
