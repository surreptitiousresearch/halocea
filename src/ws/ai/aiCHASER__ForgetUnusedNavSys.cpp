// aiCHASER::ForgetUnusedNavSys @ 0x8320BEC0
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h" // susCache, aiPlanner
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navPENMAN.h"

// Stop monitoring, then drop, the last-updated nav-system's suspicion cache.
void aiCHASER::ForgetUnusedNavSys()
{
    short nsID = susCache[this->idxLastUpdatedCache].nsID;
    navPENMAN *penMan = aiPlanner->GetPenaltyManager();
    penMan->StopMonitoringNavSys(nsID);
    susCache.Erase(this->idxLastUpdatedCache, 1);
}
