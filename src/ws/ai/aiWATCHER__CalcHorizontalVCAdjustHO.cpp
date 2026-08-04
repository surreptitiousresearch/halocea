#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/m3d/m3d_boundary.h"      // m3dVZero
#include "../../headers/ws/ai/aiWATCHER_batch_boundaries.h" // operator*, aiBRAIN_GetVCAdjustHOAmount

// aiWATCHER::CalcHorizontalVCAdjustHO @ 0x8328F268
// ?CalcHorizontalVCAdjustHO@aiWATCHER@@QBA?AUm3dV@@PBVaiBRAIN@@@Z
// Horizontal view-cover adjustment while hidden: scale the watched body's right vector (negated on
// the left side) by the observer species' VC-adjust magnitude. Only applied when there is a
// non-trivial magnitude (or mini-zoom is active), the watcher is in high hideout on a definite
// left/right side. Otherwise m3dVZero.
// (PPC struct-return: the decompiler swaps the hidden return pointer with `this`.)
m3dV aiWATCHER::CalcHorizontalVCAdjustHO(const aiBRAIN *observer) const
{
    aiWATCHER *self = const_cast<aiWATCHER *>(this);
    float amount = aiBRAIN_GetVCAdjustHOAmount(observer);
    float amountAbs = amount < 0.0f ? -amount : amount;

    if ((amountAbs >= 0.000001f || self->IsMiniZoomMode())
        && self->IsHideoutMode()
        && !self->IsInLowHO()
        && (self->IsHOSideLeft() || self->IsHOSideRight()))
    {
        m3dV bodyRight;
        self->GetBodyRight(&bodyRight);

        m3dV dir;
        if (self->IsHOSideRight())
        {
            dir = bodyRight;
        }
        else
        {
            // m3dV::operator-() — negate for the left side.
            dir.x = -bodyRight.x;
            dir.y = -bodyRight.y;
            dir.z = -bodyRight.z;
        }
        return amount * dir;
    }

    return m3dVZero;
}
