#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/navWP_ID_WEIGHT.h"

// aiPROP_FIREABLE::FindTowerManWPbyNS() @ 0x832A44F8
// ?FindTowerManWPbyNS@aiPROP_FIREABLE@@IAAPAVnavWP@@PAVaiBRAIN@@M@Z  (protected)
//
// Pick the best "tower-man" waypoint within maxDist using the owning brain's nav-system weight graph
// (center + trivial weighting functors). Returns the first candidate whose payload is a tower-man.
navWP *aiPROP_FIREABLE::FindTowerManWPbyNS(aiBRAIN *pBrain, float maxDist)
{
    navSYS *ns = aifireable_GoalNavSystem(pBrain);
    if (!ns)
        return nullptr;

    dsVECTOR<navWP_ID_WEIGHT, 8> candidates;
    aifireable_GetBestTowerManCandidates(ns, pBrain, maxDist, &candidates);

    navWP *result = nullptr;
    for (int i = 0; i < candidates.nElem; ++i)
    {
        navWP *wp = aiPlanner->GetWP((unsigned short)candidates[i].wpid);
        if (wp && wpWP_IsTwrMan(wp->wpData))
        {
            result = wp;
            break;
        }
    }
    return result;
}
