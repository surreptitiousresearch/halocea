#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/nav/navWP.h"

// aiGOAL_BASE::GetHO(short) @ 0x8329B298
// ?GetHO@aiGOAL_BASE@@SAPAVnavWP@@F@Z
//
// Static: return the high-obstacle waypoint for id `wpid` from the global planner, or null when
// the id is negative, the planner is absent, the waypoint is missing, or it is not an HO waypoint.
navWP *aiGOAL_BASE::GetHO(short wpid)
{
    if (wpid < 0)
        return nullptr;
    if (!aiPlanner)
        return nullptr; // (binary asserts here first; the null guard is the effective behaviour)

    navWP *wp = aiPlanner->GetWP(wpid);
    if (!wp)
        return nullptr;
    return wp->IsHO() ? wp : nullptr;
}
