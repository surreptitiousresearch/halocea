#include "animINST.h"
#include "objOBJ_boundary.h"
#include "anim_boundary.h"
#include "../ds/ds_assert_boundary.h"

// True when a model object should appear in the render list: it must have faces, must not be
// culled/hidden/detached (stateProc bits 0x400000 / 0x4000000 / 1), and — during a runtime update —
// must not be marked invisible (stateVis bit 0).
static bool animInstIsObjRenderable(const objOBJ *pObj, bool runtimeUpdate)
{
    if (!pObj || !pObj->nFace)
        return false;
    int stateProc = pObj->stateProc;
    if ((stateProc & 0x400000) != 0)
        return false;
    if ((stateProc & 0x4000000) != 0)
        return false;
    if ((stateProc & 1) != 0)
        return false;
    if (runtimeUpdate && (pObj->stateVis.state & 1) != 0)
        return false;
    return true;
}

// ?FillObjRendList@animINST@@QAAX_N@Z  (0x825E2280) — rebuild ppObjListRend to hold exactly the
// renderable objects from ppObjList. Two passes (count then fill) with a consistency assert.
void animINST::FillObjRendList(bool runtimeUpdate)
{
    if (!this->nObj)
        return;

    this->isObjRendVis = runtimeUpdate;

    int nVisible = 0;
    for (int i = 0; i < this->nObj; ++i)
        if (animInstIsObjRenderable(this->ppObjList[i], runtimeUpdate))
            ++nVisible;

    this->ppObjListRend = (objOBJ **)dlRealloc(
        this->ppObjListRend,
        4 * nVisible,
        "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
        0x386u);

    int nFilled = 0;
    for (int i = 0; i < this->nObj; ++i)
    {
        objOBJ *pObj = this->ppObjList[i];
        if (animInstIsObjRenderable(pObj, runtimeUpdate))
            this->ppObjListRend[nFilled++] = pObj;
    }

    if (!IGNORE_STRONG_ASSERT && nVisible != nFilled)
        STRONG_ASSERT_DUMMY().Crash(
            "nObjRend1 == nObjRend2",
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
            912,
            dsStrongAssertMessage);

    this->nObjRend = nFilled;
}
