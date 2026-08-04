#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // global aiPlanner
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h"  // gsGetPlayer
#include "../../headers/ws/ent/entENTITY.h"

// aiSTATISTICS::UpdatePlayerDirectlySeen() @ 0x832A9E08
// ?UpdatePlayerDirectlySeen@aiSTATISTICS@@IAAXXZ
//
// Clear the planner's "player directly seen" state bit (0x10000), then re-set it if any active-unit
// watcher currently both sees an enemy and has the local player as its enemy.
void aiSTATISTICS::UpdatePlayerDirectlySeen()
{
    aiPlanner->state.val &= ~0x10000u;

    const entENTITY *player = gsGetPlayer(0);
    if (!player)
        return;

    for (int i = 0; i < aiPlanner->wah.arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *watcher = aiPlanner->wah.PeekActive(i);
        if (watcher && watcher->IsEnemyVisibleNow() && watcher->HasEnemy(player))
        {
            aiPlanner->state.val |= 0x10000u;
            return;
        }
    }
}
