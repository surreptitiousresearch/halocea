#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiBRAIN.h"           // aiBRAIN::mind (aiMIND::IsIdle)
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3d_boundary.h"      // m3dVZero / m3dVUnitY
#include "../../headers/ws/ai/aiWATCHER_batch_boundaries.h" // operator*, aiBRAIN_GetVCAdjustHOAmount

// aiWATCHER::CalcVerticalVCAdjustHO @ 0x8328F138
// ?CalcVerticalVCAdjustHO@aiWATCHER@@QBA?AUm3dV@@PBVaiBRAIN@@@Z
// Vertical view-cover adjustment while hidden: scale the watched body's up vector by the observer
// species' VC-adjust magnitude, but only when the observer is not idle and this watcher is in the
// low hideout slot and NOT in mini-zoom. Otherwise m3dVZero.
// (PPC struct-return: the decompiler swaps the hidden return pointer with `this`.)
m3dV aiWATCHER::CalcVerticalVCAdjustHO(const aiBRAIN *observer) const
{
    if (!observer)
        return m3dVZero;

    aiWATCHER *self = const_cast<aiWATCHER *>(this);
    bool inLowNotMiniZoom = self->IsInLowHO() && !self->IsMiniZoomMode();

    if (!observer->mind.IsIdle() && inLowNotMiniZoom)
    {
        m3dV bodyUp = m3dVUnitY;
        entENTITY *ent = self->GetEntity();
        ent->pInst->GetDirUp(&bodyUp);
        float amount = aiBRAIN_GetVCAdjustHOAmount(observer);
        return amount * bodyUp;
    }

    return m3dVZero;
}
