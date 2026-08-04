#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiPROP_FIREABLE::FindTowerManWPwoNS() @ 0x832A4680
// ?FindTowerManWPwoNS@aiPROP_FIREABLE@@IAAPAVnavWP@@PAVaiBRAIN@@M@Z  (protected)
//
// Nearest tower-man waypoint within maxDist from the planner's global tower-man list (no nav-system
// required). Skips the search entirely for brains flagged to ignore tower-man toppling.
navWP *aiPROP_FIREABLE::FindTowerManWPwoNS(aiBRAIN *pBrain, float maxDist)
{
    if (aifireable_BrainSkipTowerMan(pBrain))
        return nullptr;

    m3dV pos = pBrain->GetPos();

    navWP *nearest = nullptr;
    navMANAGER *nm = aiPlanner->GetNavManager();
    dsVECTOR<short, 8> *towermanWPList = aifireable_TowerManWPList(nm);
    int nElem = towermanWPList->nElem;
    for (int i = 0; i < nElem; ++i)
    {
        navWP *wp = aiPlanner->GetWP((unsigned short)(*towermanWPList)[i]);
        if (wp && wpWP_IsTwrMan(wp->wpData))
        {
            float dist = wp->Dist_2(&pos);
            if (dist <= maxDist)
            {
                maxDist = dist;
                nearest = wp;
            }
        }
    }
    return nearest;
}
