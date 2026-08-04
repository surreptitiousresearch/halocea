// aiTEAMS_SYS::GetDbgColorMask @ 0x83173358
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/gs/gsTEAMS.h"

// Debug colour mask: 0xFFFFFFFF (white) when `team` is an enemy of the player,
// 0xFFDFDFDF (-2105377) otherwise.
unsigned int aiTEAMS_SYS::GetDbgColorMask(const propGAME_INFO_TEAM &team) const
{
    dsSTRID teamID;
    teamID.id = team.team.id;
    dsSTRID players;
    const dsSTRID *playersID = aiTEAMS_SYS::PlayersTeamID(&players);
    if (gsTEAMS::Instance().GetAttitude(teamID, *playersID) == gsTEAMS::ATT_ENEMY)
        return 0xFFFFFFFFu;
    return 0xFFDFDFDFu;
}
