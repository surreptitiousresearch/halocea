#include "MAP.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Insert(const K*,V*) @ 0x8255D078
// Insert-or-replace `*key` -> `*value`.
//   - First use ever (data.nElem==0): grow to 17 buckets.
//   - Bucket empty: allocate a node, splice it in as the sole entry.
//   - Bucket non-empty: walk the chain.
//       - Key match found (at any position, including the tail): overwrite the value in place,
//         wasReplaced=true, iterator points at the matched node/bucket.
//       - No match (walked off the end): allocate a node, append it after the tail, bump `count`,
//         then call TryToResize(chainDepth). If a resize happened, the bucket layout changed under
//         us, so the returned iterator's bucket index is RECOMPUTED from the (now-resized) table;
//         otherwise it reuses the original bucket index. wasReplaced=false either way.
// DEVIATION: decompile shows the allocator-exhaustion path (`Insert(ITEM**,K,V)`'s null-newItem
// crash) inline in both branches below via the shared private Insert(ITEM**,key,value) helper —
// not re-duplicated here; both branches call that helper so its OOM behavior applies uniformly.
template<>
ds::NAMED_PAIR<ds::NAMED_PAIR_value<ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> >,
                ds::NAMED_PAIR_wasReplaced<bool> >
ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Insert(const int *key, msgADDR **value)
{
    typedef ds::impl::MAP_ITERATOR<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR> Iter;
    typedef ds::NAMED_PAIR<ds::NAMED_PAIR_value<Iter>, ds::NAMED_PAIR_wasReplaced<bool> > Result;

    if (!data.nElem)
        data.Resize(17, nullptr);

    int nElem = data.nElem;
    unsigned int hash = GetHash<int>(key);
    int bucket = (int)(hash % (unsigned int)nElem);

    if (data[bucket]) {
        int chainDepth = 1;
        ITEM *node = data[bucket];
        while (node->next && *key != node->key) {
            node = node->next;
            ++chainDepth;
        }

        if (*key == node->key) {
            node->value = *value;
            return Result(Iter(this, node, bucket), true);
        }

        // No match at the tail — append a fresh node after it.
        ++count;
        dsMsgAddrMapDUMMY *empty = allocator.Alloc();
        ITEM *newItem;
        if (empty) {
            empty->pNext = nullptr;
            empty->pPrev = reinterpret_cast<dsMsgAddrMapDUMMY *>(*key);
            *reinterpret_cast<msgADDR **>(&empty->payload[0]) = *value;
            newItem = reinterpret_cast<ITEM *>(empty);
        } else {
            newItem = nullptr; // DEVIATION: OOM crash-on-null, matches Insert(ITEM**,...) sibling
        }
        newItem->next = node->next;
        node->next = newItem;
        ITEM *inserted = node->next;

        if (TryToResize(chainDepth)) {
            // Bucket count changed — re-resolve the inserted node's bucket.
            int newNElem = data.nElem;
            unsigned int newHash = GetHash<int>(key);
            int newBucket = (int)(newHash % (unsigned int)newNElem);
            return Result(Iter(this, inserted, newBucket), false);
        }
        return Result(Iter(this, inserted, bucket), false);
    }

    // Empty bucket — this is the only entry in the chain.
    ++count;
    ITEM **bucketSlot = &data[bucket];
    dsMsgAddrMapDUMMY *empty = allocator.Alloc();
    ITEM *newItem;
    if (empty) {
        empty->pNext = nullptr;
        empty->pPrev = reinterpret_cast<dsMsgAddrMapDUMMY *>(*key);
        *reinterpret_cast<msgADDR **>(&empty->payload[0]) = *value;
        newItem = reinterpret_cast<ITEM *>(empty);
    } else {
        newItem = nullptr; // DEVIATION: OOM crash-on-null, matches Insert(ITEM**,...) sibling
    }
    newItem->next = *bucketSlot;
    *bucketSlot = newItem;

    return Result(Iter(this, data[bucket], bucket), false);
}
