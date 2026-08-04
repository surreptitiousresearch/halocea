#include "scnSCENE.h"
#include "../ds/dsBIT_ARRAY_IMPL.h"

// ?_DetachInst@scnSCENE@@AAAXPAVanimINST@@@Z  (0x8253CE60, private) — unlink `pInst` from the
// live doubly-linked list and clear its bit in `isUsedInstState`. The sole-remaining-node case
// (pInst is the head with no next, i.e. the only live node) is handled separately from the
// general splice-out case.
void scnSCENE::_DetachInst(animINST *pInst)
{
    dsBIT_ARRAY_IMPL::SetNo(pInst->id, this->isUsedInstState.array, 2500);

    if (this->pFirstInst != pInst || pInst->next)
    {
        animINST *prev = pInst->prev;
        if (prev)
            prev->next = pInst->next;
        animINST *next = pInst->next;
        if (next)
            next->prev = pInst->prev;
        if (pInst == this->pFirstInst)
            this->pFirstInst = pInst->next;
        pInst->prev = nullptr;
        pInst->next = nullptr;
        pInst->pScn = nullptr;
    }
    else
    {
        pInst->prev = nullptr;
        pInst->next = nullptr;
        this->pFirstInst = nullptr;
    }
}
