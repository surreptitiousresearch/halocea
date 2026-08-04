#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/ai/aiWEAPON_boundaries.h" // WPN_TIME_KILL_HANG
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiS_PERF_LOAD.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // extern aiPLANNER *aiPlanner
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiWEAPON::StartStrikePerforming @ 0x83294138
// ?StartStrikePerforming@aiWEAPON@@IAA_NXZ
//
// Begin performing the currently-selected strike: tell the controller to perform it, register a
// strike performance-load sample with the planner's LOD system (so the load budget can gate future
// strikes), and arm the kill-hang grace timer. Returns whether a live controller was found.
bool aiWEAPON::StartStrikePerforming()
{
    propWPN_AI_USABLE *ctrl = GetCtrl(tieSelected);
    if (!ctrl)
        return false;

    ctrl->PerformStrike(tieSelected.desc);

    if (aiPlanner) {
        const wpnS_DESC *desc = tieSelected.desc;
        if (desc) {
            float loadPerformance = desc->loadPerformance;
            float timeShooting = desc->timeShooting.GetValue();
            if (loadPerformance > 0.000001f && timeShooting > 0.000001f) {
                dsTSTRING<char> brainName;
                pBrain->GetName(&brainName); // aiBRAIN_IFACE::GetName is sret (out-param) form
                aiS_PERF_LOAD load(loadPerformance, timeShooting,
                                   brainName.CStr(), tieSelected.name.CStr());
                (&aiPlanner->LOD)->AddStrikePerfLoad(load);
            }
        }
    }

    timerHackKillHang = WPN_TIME_KILL_HANG;
    return true;
}
