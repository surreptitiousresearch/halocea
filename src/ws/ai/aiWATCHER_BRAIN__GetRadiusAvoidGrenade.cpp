#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::GetRadiusAvoidGrenade @ 0x8316FB00
// ?GetRadiusAvoidGrenade@aiWATCHER_BRAIN@@QBAMXZ
float aiWATCHER_BRAIN::GetRadiusAvoidGrenade() const
{
    if (!this->IsValid())
        return 0.0f;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    return pBrain->st.pointee->perc.grAvoidRadius;
}
