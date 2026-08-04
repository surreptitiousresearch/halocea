// aiGOAL::SetNavSystem @ 0x832AF790
// ?SetNavSystem@aiGOAL@@UAAXPAVnavSYS@@_N@Z
#include "../../headers/ws/ai/aiGOAL.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner
#include "../../headers/ws/ai/aiBRAIN_DESC_abilities_boundary.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ia/iaIACTOR.h" // spDesc

// Switch the goal module's current nav system: gated on the descriptor's NS ability, drop the old
// reference / take the new one, register the new system with the planner's chaser, and (when asked)
// notify the owning coordinated group.
void aiGOAL::SetNavSystem(navSYS *ns, bool doNotify)
{
    aiBRAIN *brain = pBrain;
    if (!aiBRAIN_DESC_AbilNS(brain->spDesc.pointee))
        return;
    if (navSys == ns)
        return;

    if (navSys)
        navSys->Release(brain);
    navSys = ns;
    if (ns)
    {
        ns->AddRef(pBrain);
        (&aiPlanner->chaser)->RegisterNavSys(navSys);
    }
    if (doNotify)
    {
        aiCOORD_GROUP *grp =
            (&aiPlanner->coordinator)->GetGroup_2(pBrain);
        if (grp)
            grp->NoticeChangeNavSystem(pBrain);
    }
}
