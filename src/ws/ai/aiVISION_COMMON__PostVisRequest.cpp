#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"           // st.pointee->enemy
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"         // aiPlanner->defCDT
#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"     // Can/PostVisibilityRequest
#include "../../headers/ws/ai/aiVIS_REQ_PARAMS.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // aiPLANNER *aiPlanner

// aiVISION_COMMON::PostVisRequest @ 0x83291448
// ?PostVisRequest@aiVISION_COMMON@@MAAXXZ
// Protected virtual: post a line-of-sight (visibility) request to the planner's deferred-request
// coordinator, unless it declines. The request carries the viewer eye plus the enemy's bottom /
// random / top sight points and body up-direction; the sampled points are cached as the last
// posted request.
void aiVISION_COMMON::PostVisRequest()
{
    aiDEF_CDT_SYS *cdt = (&aiPlanner->defCDT);
    if (!cdt->CanPostVisibilityRequest(pBrain))
        return;

    aiSTATUS *st = pBrain->st.pointee;

    aiVIS_REQ_PARAMS params;
    params.FillParams(pBrain, posEye,
                      st->enemy.visQuery.points.posBottom,
                      st->enemy.visQuery.points.posRandom,
                      st->enemy.visQuery.points.posTop,
                      st->enemy.cs.dirUpBody);
    cdt->PostVisibilityRequest(params);

    paramsLastReq = st->enemy.visQuery.points;
}
