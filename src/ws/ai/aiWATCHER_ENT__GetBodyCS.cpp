#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // m3dVZero / m3dVUnitX / m3dVUnitY constants

// aiWATCHER_ENT::GetBodyCS @ 0x8328EF00
// ?GetBodyCS@aiWATCHER_ENT@@UBAXAAUm3dV@@00@Z
// Virtual const: write the tracked entity's coordinate system (position + forward + up) from its
// animated instance. Falls back to (origin, +Y, +X) when the weak ref is invalid or there is no
// instance.
void aiWATCHER_ENT::GetBodyCS(m3dV &pos, m3dV &dir, m3dV &dirUp) const
{
    aiWATCHER_ENT *self = const_cast<aiWATCHER_ENT *>(this);
    entENTITY *ent = entity.pHandle ? entity.pHandle->pPtr : nullptr;
    if (self->IsValid() && ent && ent->pInst)
    {
        ent->pInst->GetPos(&pos);
        ent->pInst->GetDir(&dir);
        ent->pInst->GetDirUp(&dirUp);
    }
    else
    {
        pos   = m3dVZero;
        dir   = m3dVUnitY;
        dirUp = m3dVUnitX;
    }
}
