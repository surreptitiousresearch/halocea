#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // mdlITRC_BASE + m3d constants/helpers + m3dMATR
#include "../../headers/ws/ai/aiWATCHER_batch_boundaries.h"  // usePROCESSOR_GetCurrentItrc
#include "../../headers/ws/m3d/m3dMATR.h"

// aiWATCHER_PLAYER::GetHideoutDir() const @ 0x8328E938
// ?GetHideoutDir@aiWATCHER_PLAYER@@UBA?AUm3dV@@XZ
// Return the hideout-facing direction for a player watcher: the negated, normalised forward axis
// (row 2) of the current hideout ITRC's base matrix, obtained through the player's use-processor.
// m3dVZero when invalid / no live ITRC; m3dVUnitX when the forward axis is degenerate.
// (PPC struct-return: the decompiler mislabels the hidden return-value pointer as `this`.)
m3dV aiWATCHER_PLAYER::GetHideoutDir() const
{
    aiWATCHER_PLAYER *self = const_cast<aiWATCHER_PLAYER *>(this);
    mdlITRC_BASE *itrc;
    if (self->IsValid()
        && useProcessor
        && (itrc = usePROCESSOR_GetCurrentItrc(useProcessor)) != nullptr
        && itrc->IsValid())
    {
        m3dMATR base;
        itrc->GetBaseMatr(&base);

        m3dV dir;
        dir.x = base.v[2].x;
        dir.y = base.v[2].y;
        dir.z = base.v[2].z;
        _m3dCheckValid(&dir);
        dir *= -1.0f;
        if (_m3dNormalize(&dir))
            return dir;
        return m3dVUnitX;
    }

    return m3dVZero;
}
