#include "MAP.h"
#include "dsMsgAddrMapDUMMY.h"

// ds::MAP<int,msgADDR*,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::Insert(ITEM**,const K&,const V&)
// @ 0x8255CC10
// Private bucket-splice helper: allocate one node from the pool, populate it, and PREPEND it onto
// the chain whose head pointer is `*bucketHead`. On allocator exhaustion the decompile shows the
// new-node pointer left null and then unconditionally dereferenced when linking it in — a
// null-deref crash-on-OOM, matching the sibling dsSTRID/DBG_CINE_ACTOR_INFO instantiation's
// documented OOM behavior for this same allocator family. Reproduced faithfully.
template<>
void ds::MAP<int, msgADDR *, ds::HASH, ds::CMP, dsNODE_CACHE_ALLOCATOR>::Insert(
    ITEM **bucketHead, const int &key, msgADDR *const &value)
{
    dsMsgAddrMapDUMMY *empty = allocator.Alloc();
    ITEM *newItem;
    if (empty) {
        empty->pNext = nullptr;
        empty->pPrev = reinterpret_cast<dsMsgAddrMapDUMMY *>(key);   // aliases ITEM::key (int)
        *reinterpret_cast<msgADDR **>(&empty->payload[0]) = value;   // aliases ITEM::value
        newItem = reinterpret_cast<ITEM *>(empty);
    } else {
        // DEVIATION: decompile shows the OOM path falling through to an unconditional dereference
        // of a null `newItem` below rather than an early return — reproduced faithfully (crash on
        // allocator exhaustion, matching the sibling MAP instantiation's OOM path).
        newItem = nullptr;
    }

    newItem->next = *bucketHead;
    *bucketHead = newItem;
}
