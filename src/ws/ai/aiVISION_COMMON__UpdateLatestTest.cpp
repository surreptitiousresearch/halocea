#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner global
#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiVIS_REQ_RES.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/AI_VIST.h"
#include "../../headers/ws/m3d/m3dRND_GEN.h"

// aiVISION_COMMON::UpdateLatestTest @ 0x832924B0
// ?UpdateLatestTest@aiVISION_COMMON@@MAAXXZ
//
// Pull a completed line-of-sight result out of the planner's def-CDT system (if one is ready): store
// the percent as the latest test, copy the free-space points, record the collision debug points, and
// arm a keep-invisible timer when the target reads as (near-)invisible.
void aiVISION_COMMON::UpdateLatestTest()
{
    aiDEF_CDT_SYS *cdt = (&aiPlanner->defCDT);
    if (!cdt->HaveVisibilityResults(pBrain))
        return;

    // Reused scratch result; the binary keeps a one-time-constructed file-scope static.
    static aiVIS_REQ_RES res;
    cdt->GetVisibilityResults(pBrain, res);

    StoreLatestTest(res.percent);
    pointsFree = res.pointsFree;
    state.val |= AI_VIST_HAS_FRESH_RESULTS; // 0x4

    aiDEBUG *dbg = &pBrain->dbg;
    if (res.percent <= 0.000001f)
    {
        dbg->vision.wasCollision   = true;
        dbg->vision.posCollisions  = res.pointsIsects;
    }
    else
    {
        dbg->vision.wasCollision        = false;
        dbg->vision.posCollisions.nEntry = 0;
    }

    if (viewLatestTest <= 10.0f)
        timerKeepInvisible = m3dRndGen.RndFloatRange(0.5f, 1.0f);
    else
        timerKeepInvisible = 0.0f;
}
