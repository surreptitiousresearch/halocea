#include "animINST.h"
#include "../ds/ds_assert_boundary.h"

// objOBJ boundary — only the owning-instance back-pointer (0x14) and the static Delete entry are
// needed here; the rest of the 220-byte model object is opaque. Completes the forward declaration
// from animINST.h without pulling the full obj header (which re-includes a conflicting animINST.h).
struct objOBJ {
    unsigned char _opaque0[0x14];
    animINST     *pInst;              // 0x14 owning animated instance
    static void   Delete(objOBJ *obj); // 0x82A7EB40  boundary
};

// anim.cpp free helper — clear an objOBJ's owning-instance back-reference. boundary.
void animSetObjInstRef(objOBJ *pObj, animINST *pInst); // boundary (0x82683xxx)

// ?DeleteObj@animINST@@QAAXPAVobjOBJ@@@Z  (0x825E2DF8) — remove a model object from this instance.
// When the instance shares its objects (state2 & 0x1000) the object is only detached; otherwise it
// is fully deleted.
void animINST::DeleteObj(objOBJ *obj)
{
    if (!IGNORE_STRONG_ASSERT && obj->pInst != this)
        STRONG_ASSERT_DUMMY().Crash(
            "pObj_->pInst == this",
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp",
            778,
            dsStrongAssertMessage);
    if ((this->state2 & 0x1000) != 0)
        animSetObjInstRef(obj, nullptr);
    else
        objOBJ::Delete(obj);
}
