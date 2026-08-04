/* objIncludePtrListID @0x82682E68 (DB: _objIncludePtrListID) — recursively index an objOBJ hierarchy into
 * a flat pointer table keyed by each object's `id`. Stores pObj at objPtrList[pObj->id], increments the
 * running count, and recurses across the circular sibling list hanging off pObj->child. Returns 0 (failure)
 * if any id is out of [0, maxNObj), otherwise 1.
 *
 * ws-engine (Saber) helper written against the C++ objOBJ class, so this is a .cpp translation unit rather
 * than a Blam C source (objOBJ's header graph is C++). */

#include "objOBJ.h"

int objIncludePtrListID(objOBJ *pObj, objOBJ **objPtrList, int maxNObj, int *nObj)
{
    if (pObj)
    {
        int iterated = 0;
        objOBJ *firstChild = pObj->child;
        int id = pObj->id;
        objOBJ *cur = firstChild;

        if (id >= maxNObj || id < 0)
            return 0;

        objPtrList[id] = pObj;
        ++*nObj;

        /* Walk the circular sibling list until it loops back to firstChild. */
        while (cur && (!iterated || firstChild != cur))
        {
            if (!objIncludePtrListID(cur, objPtrList, maxNObj, nObj))
                return 0;
            cur = cur->next;
            iterated = 1;
        }
    }
    return 1;
}
