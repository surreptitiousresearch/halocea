#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::NoticeGrenadeTerm @ 0x8316FC00
// ?NoticeGrenadeTerm@aiWATCHER_BRAIN@@QBAXPAVentENTITY@@@Z
void aiWATCHER_BRAIN::NoticeGrenadeTerm(entENTITY *entGrenade) const
{
    if (!this->IsValid())
        return;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    pBrain->mind.perc.pointee->NoticeGrenadeTerm(entGrenade);
}
