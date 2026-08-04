#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetBodyMatrNav @ 0x8316F208
// ?GetBodyMatrNav@aiWATCHER_BRAIN@@UBAXAAVm3dMATR@@@Z
// Virtual const: forward the tracked brain's nav-space body matrix into `matr`; identity when the
// weak brain ref is no longer valid. (The decompiler's unrolled 8-QWORD copy of m3dMatrIdentity is
// reproduced here as a struct assignment.)
void aiWATCHER_BRAIN::GetBodyMatrNav(m3dMATR &matr) const
{
    if (IsValid())
        brain.GetPtr()->GetBodyMatrNav(matr);
    else
        matr = m3dMatrIdentity;
}
