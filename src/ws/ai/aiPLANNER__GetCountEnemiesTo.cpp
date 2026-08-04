#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiPLANNER::GetCountEnemiesTo(const propGAME_INFO_TEAM&) const @ 0x831644C0
// Count every active unit watcher AND every active player watcher whose team is hostile to
// `teamTo` (per the planner's IsEnemy virtual).
int aiPLANNER::GetCountEnemiesTo(const propGAME_INFO_TEAM &teamTo) const
{
    aiPLANNER *self = const_cast<aiPLANNER *>(this);
    int count = 0;

    for (int i = 0; i < this->wah.arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *watcher = self->wah.PeekActive(i);
        if (watcher)
        {
            dsSTRID watcherTeam = watcher->GetTeamID()->team;
            if (self->IsEnemy(watcherTeam, teamTo.team))
                ++count;
        }
    }

    for (int i = 0; i < this->wah.arrPlayers.nElem; ++i)
    {
        aiWATCHER *player = self->wah.PeekPlayer(i);
        if (player)
        {
            dsSTRID playerTeam = player->GetTeamID()->team;
            if (self->IsEnemy(playerTeam, teamTo.team))
                ++count;
        }
    }

    return count;
}
