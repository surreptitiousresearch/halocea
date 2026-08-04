#include "objOBJ.h"
#include "objFILTER.h"
#include "obj_free_functions.h"
// 0x826829C0  ?objFind@@YAPAVobjOBJ@@PAV1@PAVobjFILTER@@@Z
// Depth-first search of the subtree rooted at `pObj` (pre-order: self, then children in
// sibling-ring order) for the first node accepted by `pObjFilter->IsUse()`. Returns nullptr
// if `pObj` is null or no node in the subtree is accepted.

objOBJ *objFind(objOBJ *pObj, objFILTER *pObjFilter)
{
    if (!pObj)
        return nullptr;

    if (pObjFilter->IsUse(pObj))
        return pObj;

    objOBJ *firstChild = pObj->child;
    int started = 0;
    objOBJ *iter = firstChild;

    while (iter && (!started || iter != firstChild)) {
        objOBJ *found = objFind(iter, pObjFilter);
        if (found)
            return found;
        iter = iter->next;
        started = 1;
    }

    return nullptr;
}
