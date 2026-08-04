#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/prop/propGAME_INFO.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiWATCHER::GetTeamID @ 0x8328EA80
// ?GetTeamID@aiWATCHER@@UBAABVpropGAME_INFO_TEAM@@XZ  (returns a const reference; expressed here as
// a pointer to match the existing aiWATCHER header declaration — ABI-identical on this target)
// Return this watcher's team handle. When the watcher is invalid or has no game-info block, the
// shared neutral-team singleton is returned. Otherwise the tracked game-info's team id is copied
// into cachedteamID (refreshed each call) and that cache is returned.
//
// team_neutral is a function-local static built once on first call (NeutralTeamID -> EnsureValid),
// matching the binary's guarded one-time init of the file-scope __team_neutral static.
propGAME_INFO_TEAM *aiWATCHER::GetTeamID()
{
    static propGAME_INFO_TEAM team_neutral;
    static bool team_neutral_inited = false;
    if (!team_neutral_inited)
    {
        team_neutral_inited = true;
        dsSTRID neutral;
        aiTEAMS_SYS::NeutralTeamID(&neutral);
        team_neutral.__vftable = &propGAME_INFO_TEAM::vftable;
        team_neutral.team.id = neutral.id;
        team_neutral.EnsureValid();
    }

    if (!IsValid())
        return &team_neutral;

    propGAME_INFO *gameInfo = pGameInfo;
    if (!gameInfo)
        return &team_neutral;

    cachedteamID.team.id = gameInfo->teamID.team.id;
    return &cachedteamID;
}
