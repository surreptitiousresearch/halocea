// aiCHASER::FindCache @ 0x8320C130
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h" // susCache
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/nav/navSYS.h"

// Locate the suspicion cache bound to nav-system `sys` (nullptr when absent).
nsSUSCACHE *aiCHASER::FindCache(navSYS *sys)
{
    if (!sys)
        return nullptr;
    if (susCache.nElem <= 0)
        return nullptr;

    int nsID = sys->nsID;
    for (int i = 0; i < susCache.nElem; ++i)
    {
        if (susCache[i].nsID == nsID)
            return &susCache[i];
    }
    return nullptr;
}
