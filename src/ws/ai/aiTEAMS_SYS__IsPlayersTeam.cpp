// aiTEAMS_SYS::IsPlayersTeam @ 0x83172F20
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"

bool aiTEAMS_SYS::IsPlayersTeam(const dsSTRID *teamID)
{
    dsSTRID players;
    return teamID->id == aiTEAMS_SYS::PlayersTeamID(&players)->id;
}
