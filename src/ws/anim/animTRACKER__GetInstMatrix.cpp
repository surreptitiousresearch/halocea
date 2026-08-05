#include "animTRACKER.h"
#include "animINST.h"
#include "../phys/physSYSTEM_BASE.h"
#include "../m3d/m3dMATR.h"
#include "../ds/ds_assert_boundary.h"

// ?GetInstMatrix@animTRACKER@@IBAXPAVanimINST@@PAVm3dMATR@@@Z  (0x82B2EB48) — protected const.
// The world transform to follow for `pInst`: prefer the physics main-body matrix while the
// instance has a live physics system; otherwise fall back to the scaled cached L2W.
void animTRACKER::GetInstMatrix(animINST *pInst, m3dMATR *out) const
{
    if (!IGNORE_STRONG_ASSERT && !pInst)
        STRONG_ASSERT_DUMMY().Crash(
            "pInst",
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim_tracker.cpp",
            500,
            empty_string);
    if (!IGNORE_STRONG_ASSERT && !out)
        STRONG_ASSERT_DUMMY().Crash(
            "out",
            "D:\\Projects\\code\\common\\src.sys\\animation\\anim_tracker.cpp",
            501,
            empty_string);

    physSYSTEM_BASE *pPhysSystem = pInst->pPhysSystem;
    if (pPhysSystem && pPhysSystem->pSystemSafe)
        pPhysSystem->GetMatrMainBody(*out);
    else
        pInst->_GetMatrL2W(out);
}
