// aiCHASER::NoticeDeath @ 0x8320C1E8
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h" // aiPlanner, t
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/nav/navSYS.h"

// Record the position where a friendly (`pBrain`) died into the appropriate suspicion cache,
// if the chaser's team is configured to react to that team's deaths.
void aiCHASER::NoticeDeath(aiBRAIN *pBrain)
{
    propGAME_INFO_TEAM *teamID = pBrain->GetTeamID();
    // aiPLANNER::teams is the opaque (size-4) aiTEAMS_SYS subobject.
    if (((aiTEAMS_SYS *)&aiPlanner->teams)->ShouldChaserNoticeDeath(*teamID))
    {
        m3dV bodyPos = pBrain->GetPos();          // virtual GetPos (vtable +448)
        this->posRecentDeath = bodyPos;
        this->timerRecentDeath = t;

        navSYS *sys = pBrain->GetNavSystem();      // virtual GetNavSystem (vtable +512)
        nsSUSCACHE *cache = FindCache(sys);
        if (cache)
            cache->AssocUpdate(ASUT_LAST_CORPSE, this->posRecentDeath);
    }
}
