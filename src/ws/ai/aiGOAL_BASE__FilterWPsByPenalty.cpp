#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navPENMAN.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/navWP_ID_WEIGHT.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiGOAL_BASE::FilterWPsByPenalty(...) @ 0x8329BF80
// ?FilterWPsByPenalty@aiGOAL_BASE@@SAXMABV?$dsVECTOR@UnavWP_ID_WEIGHT@@$07@@AAV2@@Z
//
// For each waypoint in `src`, add its current dynamic navigation penalty to the incoming weight;
// keep those whose penalty is under `maxWeight` in `dst`.
void aiGOAL_BASE::FilterWPsByPenalty(float maxWeight,
                                     const dsVECTOR<navWP_ID_WEIGHT, 8> &src,
                                     dsVECTOR<navWP_ID_WEIGHT, 8> &dst)
{
    const int cap = dst.allocated;
    dst.nElem = 0;
    if (src.nElem > cap)
        dst.Realloc(src.nElem);

    navMANAGER *navMgr = aiPlanner->GetNavManager();
    navPENMAN *penMgr = aiPlanner->GetPenaltyManager();

    const int srcCount = src.nElem;
    for (int i = 0; i < srcCount; ++i)
    {
        const short wpid = src[i].wpid;
        const float baseWeight = src[i].weight;

        navWP *wp = navMgr->GetWP(wpid);
        float penalty = penMgr->GetPenalty(wp);
        if (penalty < maxWeight)
        {
            navWP_ID_WEIGHT e;
            e.weight = penalty + baseWeight;
            e.wpid = wpid;
            dst.PushBack(e);
        }
    }
}
