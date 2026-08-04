#include "../../headers/ws/ai/aiAPPROACH.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/ent/entENTITY.h"
#include <math.h>

extern int gsGetPlayerCount();
extern entENTITY *gsGetPlayer(int index);

// aiAPPROACH::CheckDistance() @ 0x8329A8D8
// ?CheckDistance@aiAPPROACH@@IAAXXZ
//
// Re-evaluate the SOMEONE_IN_ZONE flag: with a valid radius, test the configured set of watched
// entities (player 0, remaining players, and/or the on-screen ally) against CheckDistance(entity).
// A radius of -1 means "always in zone".
void aiAPPROACH::CheckDistance()
{
    unsigned int st = state.val;

    // radius == -1 (within epsilon): treat as permanently in-zone.
    if (fabsf(-1.0f - radius) < 0.000001f)
    {
        state.val = st | AI_APST_SOMEONE_IN_ZONE;
        return;
    }

    bool inZone = false;

    if (st & AI_APST_NOTICE_PLAYER1)
        inZone = CheckDistance(gsGetPlayer(0));

    if (!inZone && (state.val & AI_APST_NOTICE_PLAYER2))
    {
        for (int i = 1; i < gsGetPlayerCount(); ++i)
            inZone |= CheckDistance(gsGetPlayer(i));
    }

    if (!inZone && (state.val & AI_APST_NOTICE_ALLY) != 0)
    {
        aiBRAIN_IFACE *ally = aiPlanner->FindWithOnScreenIdx(0);
        inZone = CheckDistance(ally);
    }

    if (inZone)
        state.val |= AI_APST_SOMEONE_IN_ZONE;
    else
        state.val &= ~AI_APST_SOMEONE_IN_ZONE;
}
