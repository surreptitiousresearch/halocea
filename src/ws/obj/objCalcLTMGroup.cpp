#include "objOBJ.h"
#include "obj_free_functions.h"
// 0x82B0B6F0  ?objCalcLTMGroup@@YAXPAVobjOBJ@@@Z
// Recompute local-to-model matrices for the object group rooted at `pObjFrom`: updates this
// node's matrLT via objCalcNodeLTM(), then recurses into each child whose objOBJ::state has
// bit 0x2 set. Skips the whole subtree if stateProc bit 0x800 (locked/frozen group) is set.
// The child walk is a circular sibling ring (next-linked), guarded by `started` so a single
// node ring (child->next == child) is still visited exactly once.

void objCalcLTMGroup(objOBJ *pObjFrom)
{
    if ((pObjFrom->stateProc & 0x800) != 0)
        return;

    objCalcNodeLTM(pObjFrom);

    objOBJ *firstChild = pObjFrom->child;
    int started = 0;
    objOBJ *iter = firstChild;

    while (iter && (!started || iter != firstChild)) {
        if ((iter->state & 2) != 0)
            objCalcLTMGroup(iter);
        iter = iter->next;
        started = 1;
    }
}
