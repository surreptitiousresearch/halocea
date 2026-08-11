#include "objOBJ.h"
#include "obj_free_functions.h"
// @0x82B0B780  ?objCalcLTMGroupExcl@@YAXPAVobjOBJ@@@Z
// Exclusive-locked variant of objCalcLTMGroup(): recomputes this node's matrLT only if
// objOBJ::state bit 0x2 is NOT already set (i.e. skip if already validated), but then always
// recurses into every child unconditionally (no per-child state-bit gate) — unlike
// objCalcLTMGroup, which gates the recursive call on the child's own state bit.

void objCalcLTMGroupExcl(objOBJ *pObjFrom)
{
    if ((pObjFrom->stateProc & 0x800) != 0)
        return;

    if ((pObjFrom->state & 2) == 0)
        objCalcNodeLTM(pObjFrom);

    objOBJ *firstChild = pObjFrom->child;
    int started = 0;
    objOBJ *iter = firstChild;

    while (iter && (!started || iter != firstChild)) {
        objCalcLTMGroupExcl(iter);
        iter = iter->next;
        started = 1;
    }
}
