#include "../../headers/ws/ai/aiWATCHER_PLAYER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3dMATR.h"

extern m3dMATR m3dMatrIdentity; // ?m3dMatrIdentity@@3Vm3dMATR@@A (m3d global constant)  boundary

// aiWATCHER_PLAYER::GetBodyMatrNav @ 0x8328EC60
// ?GetBodyMatrNav@aiWATCHER_PLAYER@@UBAXAAVm3dMATR@@@Z
// Virtual const: build the nav-space body matrix — the player instance's local-to-world matrix
// with its translation replaced by the nav-space body position. Identity when the weak player ref
// is no longer valid.
void aiWATCHER_PLAYER::GetBodyMatrNav(m3dMATR &matr) const
{
    aiWATCHER_PLAYER *self = const_cast<aiWATCHER_PLAYER *>(this);
    if (self->IsValid())
    {
        entENTITY *ent = player.pHandle ? player.pHandle->pPtr : nullptr;
        ent->pInst->GetMatrL2W(&matr);
        m3dV navPos;
        self->GetBodyPosNav(&navPos);
        matr.Translate(&navPos, 0);
    }
    else
    {
        matr = m3dMatrIdentity;
    }
}
