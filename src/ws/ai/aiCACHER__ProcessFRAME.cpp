#include "../../headers/ws/ai/aiCACHER.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"   // global aiPlanner
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCHASER_boundary.h"   // gsGetPlayerCount
#include "../../headers/ws/ap/apCOUNTER_TIME.h"  // transitively provides apCOUNTER_TIME_CALL_WRP

// aiCACHER::ProcessFRAME() @ 0x832AA048
// ?ProcessFRAME@aiCACHER@@UAAXXZ
//
// Per-frame watcher/chaser tick (timed). If the planner is flagged as needing its player set
// (state bit 0x8), try to (re)acquire players and clear the bit once obtained; otherwise raise the
// bit whenever the cached player-watcher count no longer matches the live player count. Then drive
// the watcher subsystem and chaser using this actor's elapsed frame time.
void aiCACHER::ProcessFRAME()
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/planner/cacher", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    if (!aiPlanner)
        return;

    if ((aiPlanner->state.val >> 3) & 1)
    {
        if (aiPlanner->wah.TryGettingPlayers())
            aiPlanner->state.val &= 0xFFFFFFF7u; // clear "need players" bit 0x8
    }
    else if (aiPlanner->wah.arrPlayers.nElem != gsGetPlayerCount())
    {
        aiPlanner->state.val |= 8;
    }

    float dt = GetElapsedTime();
    aiPlanner->wah.ProcessFrame(dt);

    aiCHASER *chaser = (&aiPlanner->chaser);
    chaser->UpdateViewPositions();
    chaser->UpdateTimers(dt);
}
