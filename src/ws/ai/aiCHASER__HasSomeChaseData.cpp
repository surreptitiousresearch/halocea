// aiCHASER::HasSomeChaseData @ 0x8320C328
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h"
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"

int aiCHASER::HasSomeChaseData(aiBRAIN *pBrain)
{
    propGAME_INFO_TEAM *teamID = pBrain->GetTeamID();
    // aiPLANNER::teams is the opaque (size-4) aiTEAMS_SYS subobject.
    if (((aiTEAMS_SYS *)&aiPlanner->teams)->IsAcceptedByChaser(*teamID))
    {
        navSYS *sys = pBrain->GetNavSystem();
        nsSUSCACHE *cache = FindCache(sys);
        if (cache)
            return cache->HasAnyBetterThan(ASUT_RANDOM);
    }
    return 0;
}
