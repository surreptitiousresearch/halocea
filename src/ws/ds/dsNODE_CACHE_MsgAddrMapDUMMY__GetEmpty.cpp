#include "dsNODE_CACHE.h"
#include "dsMsgAddrMapDUMMY.h"

// dsNODE_CACHE<...msgADDR-map DUMMY>::GetEmpty @ 0x8255BC10
// Return a free node, growing the pool first if the free list is empty; the node is
// unlinked from the free list before being handed out.
// CORRECTED (msgADDR-map batch, DB-verified via ds::PTR_LIST<DUMMY,...>::Erase's real mangle):
// Erase takes its ITERATOR BY VALUE and RETURNS the unlinked T* — not `void Erase(const ITERATOR*)`
// as an earlier pass on this generic PTR_LIST template had it; call site fixed to match.
template<>
dsMsgAddrMapDUMMY *dsNODE_CACHE<dsMsgAddrMapDUMMY>::GetEmpty()
{
    if (!this->freeNodeList.pHead)
        this->AddNodes();
    dsMsgAddrMapDUMMY *emptyNode = this->freeNodeList.pHead;
    this->freeNodeList.Erase(ds::PTR_LIST<dsMsgAddrMapDUMMY>::ITERATOR(emptyNode));
    return emptyNode;
}
