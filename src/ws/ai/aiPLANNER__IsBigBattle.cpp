#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiPLANNER::IsBigBattle() const @ 0x83164AB8  (virtual)
// True when the local player's team is up against a "big" number of enemies. The disassembly
// computes ((count >= 0) + (count <= 3)) & 1 which — because an enemy count is always >= 0 —
// reduces exactly to (count > 3).
bool aiPLANNER::IsBigBattle() const
{
    dsSTRID playersTeamId;
    aiTEAMS_SYS::PlayersTeamID(&playersTeamId);

    propGAME_INFO_TEAM team;      // ctor installs the team vtable
    team.team = playersTeamId;
    team.EnsureValid();

    int enemyCount = GetCountEnemiesTo(team);
    return enemyCount > 3;
}
