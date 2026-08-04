#include "dsNODE_CACHE.h"

// boundary — ds::MAP<dsSTRID,DBG_CINE_ACTOR_INFO,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a debug cinematic-actor hash-map node record. Opaque here; only linked and handed out by pointer.
struct dsCineActorMapDUMMY;

// dsNODE_CACHE<...cine-actor-map DUMMY>::GetEmpty @ 0x823B8EA0
// Return a free node, growing the pool first if the free list is empty; the node is unlinked
// from the free list before being handed out. (The original inlines PTR_LIST::Erase.)
// CORRECTED (msgADDR-map batch, DB-verified via ds::PTR_LIST<DUMMY,...>::Erase's real mangle):
// Erase takes its ITERATOR BY VALUE and RETURNS the unlinked T* — not `void Erase(const ITERATOR*)`
// as an earlier pass on this generic PTR_LIST template had it; call site fixed to match.
template<>
dsCineActorMapDUMMY *dsNODE_CACHE<dsCineActorMapDUMMY>::GetEmpty()
{
    if (!this->freeNodeList.pHead)
        this->AddNodes();
    dsCineActorMapDUMMY *emptyNode = this->freeNodeList.pHead;
    this->freeNodeList.Erase(ds::PTR_LIST<dsCineActorMapDUMMY>::ITERATOR(emptyNode));
    return emptyNode;
}
