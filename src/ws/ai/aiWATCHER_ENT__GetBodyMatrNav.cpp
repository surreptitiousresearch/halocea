#include "../../headers/ws/ai/aiWATCHER_ENT.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/m3d/m3dMATR.h"

extern m3dMATR m3dMatrIdentity; // ?m3dMatrIdentity@@3Vm3dMATR@@A (m3d global constant)  boundary

// aiWATCHER_ENT::GetBodyMatrNav @ 0x8328EE38
// ?GetBodyMatrNav@aiWATCHER_ENT@@UBAXAAVm3dMATR@@@Z
// Virtual const: forward the tracked entity instance's local-to-world matrix into `matr`. Identity
// when the weak ref is invalid or the entity has no animated instance.
void aiWATCHER_ENT::GetBodyMatrNav(m3dMATR &matr) const
{
    aiWATCHER_ENT *self = const_cast<aiWATCHER_ENT *>(this);
    entENTITY *ent = entity.pHandle ? entity.pHandle->pPtr : nullptr;
    if (self->IsValid() && ent && ent->pInst)
    {
        ent->pInst->GetMatrL2W(&matr);
    }
    else
    {
        matr = m3dMatrIdentity;
    }
}
