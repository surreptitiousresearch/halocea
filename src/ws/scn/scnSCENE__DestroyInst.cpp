#include "scnSCENE.h"

// ?DestroyInst@scnSCENE@@QAAXPAVanimINST@@@Z  (0x8253CF18) — release collision/collision-anim
// state, detach `pInst` from the live list, reset it to its cleared state, then push it onto the
// free list (reusing the `next` link).
void scnSCENE::DestroyInst(animINST *pInst)
{
    if (!pInst)
        return;

    pInst->FreeCollidables();
    if (this->pCollAnim)
        this->pCollAnim->Invalidate(pInst, 1);

    this->_DetachInst(pInst);
    animINST::Clear(pInst);

    pInst->id = pInst - this->instList;
    pInst->next = this->pFirstFree;
    this->pFirstFree = pInst;
}
