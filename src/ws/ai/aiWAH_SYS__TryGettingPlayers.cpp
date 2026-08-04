#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWAH_SYS_boundaries.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiWAH_SYS::TryGettingPlayers() @ 0x83172218
// ?TryGettingPlayers@aiWAH_SYS@@QAA_NXZ
//
// Rebuild arrPlayers from the current player set: clear it, reserve room, then Demand a holder
// for each player and record its watcher. Returns false when there are no players.
bool aiWAH_SYS::TryGettingPlayers()
{
    if (!gsGetPlayerCount())
        return false;

    const int playerCount = gsGetPlayerCount();
    arrPlayers.nElem = 0;
    if (playerCount > arrPlayers.allocated)
        arrPlayers.Realloc(playerCount);

    for (int i = 0; i < playerCount; ++i)
    {
        aiWTC_HOLDER *holder = Demand(gsGetPlayer(i));
        if (holder)
            arrPlayers.PushBack(holder->watcher);
    }
    return true;
}
