// aiCHASER::GetBestViewPoints @ 0x8320C560
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/m3d/m3dV.h"

bool aiCHASER::GetBestViewPoints(aiBRAIN *pBrain, dsVECTOR<short, 8> &wps)
{
    wps.nElem = 0;

    navSYS *sys = pBrain->GetNavSystem();
    nsSUSCACHE *cache = FindCache(sys);
    if (cache)
    {
        cache->ConsiderAddingRandom();
        m3dV pos;
        while (cache->CopyBestView(&wps, &pos))
        {
            if (wps.nElem)
                return true;
        }
    }
    return false;
}
