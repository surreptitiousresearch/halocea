#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"

// aiPERC::ApplySingleSelectionWarPeace @ 0x8329CE70
// ?ApplySingleSelectionWarPeace@aiPERC@@IAAXXZ
//
// Decide the mind's "battle" flag (mind bit 0x80) for an ungrouped unit. A scripted force override
// wins outright; an idle unit is at peace; otherwise the unit is at war if any active watcher on a
// non-neutral team is itself non-idle, or (failing that) if its own enemy module reports war.
void aiPERC::ApplySingleSelectionWarPeace()
{
    aiSTATUS *st = pBrain->st.pointee;
    unsigned int gen = st->gen.val;
    bool forced;
    if (gen & 1)              { forced = true; st->mind.val |= 0x80u; }
    else if ((gen >> 1) & 1)  { forced = true; st->mind.val &= ~0x80u; }
    else                        forced = false;
    if (forced)
        return;

    aiSTATUS *stMind = pBrain->st.pointee;
    if (!pBrain->mind.IsIdle()) {
        const propGAME_INFO_TEAM *selfTeam = pBrain->GetTeamID();
        aiTEAMS_SYS *teams = reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams);
        int n = aiPlanner->wah.arrUnits.nElem;
        bool atWar = false;
        for (int i = 0; i < n; ++i) {
            aiWATCHER_BRAIN *w = aiPlanner->wah.PeekActive(i);
            const propGAME_INFO_TEAM *wTeam = w->GetTeamID();
            bool mutuallyNeutral = teams->IsNeutral(*selfTeam, *wTeam)
                                && teams->IsNeutral(*wTeam, *selfTeam);
            if (!mutuallyNeutral && !w->IsIdle()) {
                atWar = true;
                break;
            }
        }
        if (!atWar)
            atWar = pBrain->mind.enemy.pointee->IsInWar();
        if (atWar)
            stMind->mind.val |= 0x80u;
        else
            stMind->mind.val &= ~0x80u;
    } else {
        stMind->mind.val &= ~0x80u;
    }
}
