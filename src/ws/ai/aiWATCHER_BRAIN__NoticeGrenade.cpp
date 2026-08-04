#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::NoticeGrenade @ 0x8316FB80
// ?NoticeGrenade@aiWATCHER_BRAIN@@QBAXPAVentENTITY@@@Z
void aiWATCHER_BRAIN::NoticeGrenade(entENTITY *entGrenade) const
{
    if (!this->IsValid())
        return;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    pBrain->mind.perc.pointee->NoticeGrenade(entGrenade);
}
