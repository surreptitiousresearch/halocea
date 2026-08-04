#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiPERC::ApplyGroupSelectionWarPeace @ 0x8329D018
// ?ApplyGroupSelectionWarPeace@aiPERC@@IAAXAAVaiCOORD_GROUP@@@Z
//
// Decide the mind's "battle" flag (mind bit 0x80) for a grouped unit. A scripted force override on
// this unit wins; otherwise any group member's force-war/force-peace override propagates to the
// whole group; failing that, if the unit has an enemy and any member is already at war the unit
// goes to war; otherwise fall back to the single-unit decision.
void aiPERC::ApplyGroupSelectionWarPeace(aiCOORD_GROUP &grp)
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
    int count = grp.GetMembersCount();

    bool war = false;
    bool decided = false;
    for (int i = 0; i < count; ++i) {
        unsigned int memberGen = grp.GetBrain(i)->st.pointee->gen.val;
        if (memberGen & 1) {            // a member is force-war
            war = true;
            decided = true;
            break;
        }
        if ((memberGen >> 1) & 1) {      // a member is force-peace
            stMind->mind.val &= ~0x80u;
            return;
        }
    }

    if (!decided && !pBrain->mind.enemy.pointee->Is(nullptr)) {
        for (int i = 0; i < count; ++i) {
            if ((static_cast<unsigned int>(grp.GetBrain(i)->st.pointee->mind.val) >> 7) & 1) {
                war = true;
                decided = true;
                break;
            }
        }
    }

    if (decided && war) {
        stMind->mind.val |= 0x80u;
        return;
    }
    ApplySingleSelectionWarPeace();
}
