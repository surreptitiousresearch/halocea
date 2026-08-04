#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"

// aiPLANNER::GetActiveUnitsCnt(const propGAME_INFO_TEAM&) const @ 0x83169E28
// Count the active AI-unit watchers whose team matches `team`.
int aiPLANNER::GetActiveUnitsCnt(const propGAME_INFO_TEAM &team) const
{
    aiPLANNER *self = const_cast<aiPLANNER *>(this);
    int count = 0;

    for (int i = 0; i < this->wah.arrUnits.nElem; ++i)
    {
        aiWATCHER_BRAIN *watcher = self->wah.PeekActive(i);
        if (watcher && team.team.id == watcher->GetTeamID()->team.id)
            ++count;
    }

    return count;
}
