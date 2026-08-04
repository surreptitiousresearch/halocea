// aiTEAMS_SYS::IsAcceptedByChaser @ 0x831732F0
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/gs/gsTEAMS.h"

bool aiTEAMS_SYS::IsAcceptedByChaser(const propGAME_INFO_TEAM &team) const
{
    dsSTRID teamID;
    teamID.id = team.team.id;
    dsSTRID players;
    const dsSTRID *playersID = aiTEAMS_SYS::PlayersTeamID(&players);
    return gsTEAMS::Instance().GetAttitude(teamID, *playersID) == gsTEAMS::ATT_ENEMY;
}
