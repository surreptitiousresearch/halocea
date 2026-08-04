#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner global
#include "../../headers/ws/ai/aiCHEAP_VISION.h"
#include "../../headers/ws/ai/AI_VIST.h"

// aiVISION_COMMON::ExportAll @ 0x83292260
// ?ExportAll@aiVISION_COMMON@@IAAXXZ
//
// Publish the current visibility state into the status enemy block. Always writes the integer
// view-percent; when a fresh result flag is set, clears it, feeds the cheap-vision cache (unless the
// result itself came from the cheap cache), and copies the sight points / free points / success flag
// into the enemy's visResult.
void aiVISION_COMMON::ExportAll()
{
    pBrain->st.pointee->perc.viewPrecent = (int)viewPercent;

    if (state.val & AI_VIST_HAS_FRESH_RESULTS) // 0x4
    {
        state.val &= ~AI_VIST_HAS_FRESH_RESULTS;
        if (state.val & AI_VIST_GRABBED_CHEAP) // 0x8
            state.val &= ~AI_VIST_GRABBED_CHEAP;
        else
            (&aiPlanner->CheaVis)
                ->PostVisibilityInfo(pBrain, posEye, viewPercent);

        aiSTATUS *st = pBrain->st.pointee;
        st->enemy.visResult.points      = paramsLastReq;
        st->enemy.visResult.pointsFree  = pointsFree;
        st->enemy.visResult.isSucceeded = viewLatestTest >= 1.0f;
    }
}
