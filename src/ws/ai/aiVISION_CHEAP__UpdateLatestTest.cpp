#include "../../headers/ws/ai/aiVISION_CHEAP.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner global
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/AI_VIST.h"

// aiVISION_CHEAP::UpdateLatestTest @ 0x83291A48
// ?UpdateLatestTest@aiVISION_CHEAP@@MAAXXZ
//
// Cheap variant: instead of posting a CDT probe, read the best cached view-percent for this brain
// out of the planner's cheap-vision cache and store it as the latest test result.
void aiVISION_CHEAP::UpdateLatestTest()
{
    aiCHEAP_VISION *cheaVis = (&aiPlanner->CheaVis);
    StoreLatestTest(cheaVis->CalculateViewPercent(pBrain));
    state.val |= AI_VIST_GRABBED_CHEAP;      // 0x8
    paramsLastReq = pBrain->st.pointee->enemy.visQuery.points;
    state.val |= AI_VIST_HAS_FRESH_RESULTS;  // 0x4
}
