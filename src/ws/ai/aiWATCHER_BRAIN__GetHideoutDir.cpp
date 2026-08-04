#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/m3d/m3dMATR.h"

// aiWATCHER_BRAIN::GetHideoutDir() const @ 0x83172C78
// ?GetHideoutDir@aiWATCHER_BRAIN@@UBA?AUm3dV@@XZ
//
// Return the hideout-facing direction: the negated, normalised forward axis (row 2) of the current
// hideout ITRC's base matrix. m3dVZero when not in hideout mode / no live ITRC; m3dVUnitX when the
// forward axis is degenerate. (PPC struct-return: the decompiler mislabels the hidden return-value
// pointer as `this` — reconstructed here as a by-value m3dV result.)
m3dV aiWATCHER_BRAIN::GetHideoutDir() const
{
    if (!IsHideoutMode())
        return m3dVZero;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;

    dsSHARED_PTR<mdlITRC_BASE, 0, Deleter<mdlITRC_BASE> > itrc;
    if (pBrain && pBrain->body.pointee->GetCurrentItrc(&itrc) && itrc.pointee->IsValid())
    {
        m3dMATR base;
        itrc.pointee->GetBaseMatr(&base);

        m3dV dir;
        dir.x = base.v[2].x;
        dir.y = base.v[2].y;
        dir.z = base.v[2].z;
        _m3dCheckValid(&dir);
        dir *= -1.0f;
        if (!_m3dNormalize(&dir))
            dir = m3dVUnitX;
        return dir;
    }

    return m3dVZero;
}
