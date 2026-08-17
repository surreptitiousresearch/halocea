/* ?DetachObj@animINST@@QAAXPAVobjOBJ@@@Z @0x825E1238 */
#include "animINST.h"
#include "objOBJ_boundary.h"
#include "../ds/ds_assert_boundary.h"

// ?DetachObj@animINST@@QAAXPAVobjOBJ@@@Z  (0x825E1238) — remove `pObj` from this instance's object
// tables (linear list slot, root pointer, and the render list) without deleting the object.
void animINST::DetachObj(objOBJ *pObj)
{
    if (!IGNORE_STRONG_ASSERT && pObj->pInst != this)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "_pObj->pInst == this",
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
            792,
            empty_string);

    int id = pObj->id;
    if (id >= 0 && id < this->nObj)
        this->ppObjList[id] = nullptr;
    if (!pObj->parent)
        this->pObj = nullptr;

    for (int i = 0; i < this->nObjRend; ++i)
    {
        objOBJ **ppObjListRend = this->ppObjListRend;
        if (ppObjListRend[i] == pObj)
        {
            // swap-remove: move the last entry into this slot, then shrink and clear the tail.
            ppObjListRend[i] = ppObjListRend[this->nObjRend - 1];
            this->ppObjListRend[--this->nObjRend] = nullptr;
        }
    }
}
