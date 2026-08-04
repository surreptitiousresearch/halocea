#include "objOBJ.h"
#include "objOBJ_ITER_SAFE.h"
#include "obj_boundary.h"
// 0x826621B0  ?Duplicate@objOBJ@@QAAPAV1@PAV1@@Z
// Deep-copy this object (and, recursively, its whole child subtree), parenting the clone under
// `pPar` (inserted at the tail of pPar's child ring). Returns the new clone, or nullptr if any
// allocation in the subtree failed.

objOBJ *objOBJ::Duplicate(objOBJ *pPar)
{
    objOBJ_ITER_SAFE childIter(this); // snapshot children before cloning mutates the tree

    objOBJ *clone = _DuplicateTpl2Inst(1);
    if (!clone) {
        dlFree(childIter.childList.pData);
        return nullptr;
    }

    if (pPar) {
        clone->parent = pPar;
        objOBJ *firstChild = pPar->child;
        if (firstChild) {
            clone->next = firstChild;
            clone->prev = firstChild->prev;
            firstChild->prev->next = clone;
            firstChild->prev = clone;
        } else {
            pPar->child = clone;
        }
    }

    for (childIter.cur = 0; childIter.cur < childIter.childList.nElem; ++childIter.cur) {
        objOBJ *childToClone = childIter.childList[childIter.cur];
        if (!childToClone->Duplicate(clone)) {
            dlFree(childIter.childList.pData);
            return nullptr;
        }
    }

    dlFree(childIter.childList.pData);
    return clone;
}
