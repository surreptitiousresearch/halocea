#include "objOBJ.h"
#include "obj_free_functions.h"
// 0x826848D0  ?objMakePtrListID@@YAHPAVobjOBJ@@PAPAV1@H@Z
// Collect up to `maxNObj` node pointers (starting at `pObjParent`) into `objPtrList` and
// return the number of entries written. Thin wrapper over the recursive
// objIncludePtrListID() helper (sibling function — boundary, not reversed in this batch).

int objMakePtrListID(objOBJ *pObjParent, objOBJ **objPtrList, int maxNObj)
{
    int count = 0;
    objIncludePtrListID(pObjParent, objPtrList, maxNObj, &count);
    return count;
}
