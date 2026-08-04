#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // aiENEMY_BASE

// ai08 planner singleton (?aiPlanner@@3PAVaiPLANNER@@A). boundary data.
extern aiPLANNER *aiPlanner;

// aiGRASS::CanThrowGrenade @ 0x83183980
// ?CanThrowGrenade@aiGRASS@@QBA_NPBVaiBRAIN@@@Z  (const)
//
// Decide whether `pBrain` is currently allowed to lob a grenade at its tracked enemy peeker:
// the peeker must be valid, the descriptor must permit grenading this peeker type (a
// descriptor flag gates player peekers), and — for a unit in a coordinated group that is not
// itself flagged — the peeker must be actively smoking out one of our tracked victims.
bool aiGRASS::CanThrowGrenade(const aiBRAIN *pBrain) const
{
    if (!pBrain)
        return false;

    aiENEMY_BASE *enemy = pBrain->mind.enemy.pointee;
    if (!enemy)
        return false;

    aiWATCHER *peeker = enemy->GetPeeker();
    if (!peeker || !peeker->IsValid())
        return false;

    // Descriptor grenade-permission flags (verified against disasm: lbz 0x1F9 / lbz 0x1F8).
    const unsigned char *desc = reinterpret_cast<const unsigned char *>(pBrain->spDesc.pointee);
    unsigned char gatePlayerPeeker = desc[0x1F9]; // gate: peeker must be a player
    unsigned char nonHideoutResult = desc[0x1F8]; // result when peeker not in hideout mode

    if (gatePlayerPeeker && !peeker->IsPlayer())
        return false;

    aiCOORDINATOR *coord = (&aiPlanner->coordinator);
    if (!coord->GetGroup(const_cast<aiBRAIN *>(pBrain)) ||
        ((pBrain->st.pointee->gen.val >> 20) & 1) != 0)
    {
        return true;
    }

    if (!peeker->IsHideoutMode())
        return nonHideoutResult != 0;

    for (ds::LIST<aiWATCHER *>::DATA *node = this->victims.head; node; node = node->next)
    {
        if (node->val == peeker)
            return true;
    }
    return false;
}
