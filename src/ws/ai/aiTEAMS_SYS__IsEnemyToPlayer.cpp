// aiTEAMS_SYS::IsEnemyToPlayer @ 0x831730E8
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/gs/gsTEAMS.h"

bool aiTEAMS_SYS::IsEnemyToPlayer(const propGAME_INFO_TEAM &teamFrom) const
{
    dsSTRID from;
    from.id = teamFrom.team.id;
    dsSTRID players;
    const dsSTRID *playersID = aiTEAMS_SYS::PlayersTeamID(&players);
    return gsTEAMS::Instance().GetAttitude(from, *playersID) == gsTEAMS::ATT_ENEMY;
}
