// aiCHASER::RegisterNavSys @ 0x8320CC10
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h"
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/nav/navPENMAN.h"

void aiCHASER::RegisterNavSys(navSYS *sys)
{
    if (sys && !FindCache(sys))
    {
        // Local cache is copied into the global vector, then destructed on scope exit
        // (frees its point storage + releases its name buffer — inlined in the binary).
        nsSUSCACHE cache(sys->nsID);
        susCache.PushBack(cache);

        navPENMAN *penManager = aiPlanner->penManager;
        penManager->StartMonitoringNavSys(sys->nsID);
    }
}
