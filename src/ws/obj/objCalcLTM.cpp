#include "objOBJ.h"
#include "../anim/animINST.h"
#include "obj_free_functions.h"
// 0x82661040  ?objCalcLTM@@YAXPAVobjOBJ@@@Z
// General-purpose LTM propagation entry point. Unlike objCalcLTMGroup/objCalcLTMGroupExcl,
// this is null-tolerant on `pObjFrom` and additionally skips a node if its owning animINST
// has state2 bit 0x4 set (externally-driven / frozen LTM). Otherwise recomputes this node's
// matrLT via objCalcNodeLTM() and always recurses into every child.

void objCalcLTM(objOBJ *pObjFrom)
{
    if (!pObjFrom)
        return;

    if ((pObjFrom->stateProc & 0x800) != 0)
        return;

    animINST *pInst = pObjFrom->pInst;
    if (pInst && (pInst->state2 & 4) != 0)
        return;

    objCalcNodeLTM(pObjFrom);

    objOBJ *firstChild = pObjFrom->child;
    int started = 0;
    objOBJ *iter = firstChild;

    while (iter && (!started || iter != firstChild)) {
        objCalcLTM(iter);
        iter = iter->next;
        started = 1;
    }
}
