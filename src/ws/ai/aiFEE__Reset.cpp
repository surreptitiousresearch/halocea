// aiFEE::Reset @ 0x8321F210
#include "../../headers/ws/ai/aiFEE.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPlanner

void aiFEE::Reset(aiWTC_HOLDER *wh)
{
    // Release any previous holder back to the watcher subsystem.
    if (hold)
        aiPlanner->wah.Forgo(hold, -1);

    hold = wh;
    if (wh)
        ++wh->counter;

    // Cache whether the (new) holder's watcher is a player.
    isPlayer = hold && hold->watcher->IsPlayer();
}
