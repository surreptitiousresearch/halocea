#include <stdint.h>
#include "animINST.h"
#include "anim_boundary.h"
#include <string.h>

// objOBJ boundary — only the node-count query is needed here; the rest of the model object is
// opaque. Completes the forward declaration from animINST.h without pulling the full obj header
// (which re-includes a conflicting Blam animINST.h).
struct objOBJ {
    int GetNObj(void *filter); // 0x... boundary — count of objects in this subtree
};

// ?FillLinearList@animINST@@QAAXXZ  (0x825E1358) — flatten the model-object tree rooted at pObj
// into two parallel arrays: a linear objOBJ* list and a per-object camera-visibility mask table.
void animINST::FillLinearList()
{
    int nObj = this->pObj->GetNObj(nullptr);
    this->nObj = nObj;
    if (nObj)
    {
        this->ppObjList = (objOBJ **)dlRealloc(
            this->ppObjList,
            4 * nObj,
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
            0x342);
        memset(this->ppObjList, 0, 4 * this->nObj);
        objMakePtrListID(this->pObj, this->ppObjList, this->nObj);

        this->pObjCamMask = (int64_t *)dlRealloc(
            this->pObjCamMask,
            8 * this->nObj,
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
            0x346);
        memset(this->pObjCamMask, 0, 8 * this->nObj);
    }
}
