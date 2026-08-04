// aiTEAMS_SYS::IsPredefinedTeam @ 0x83172FB0
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/gs/gsTEAMS.h"

// True when teamID is one of the two reserved teams (players or neutral).
bool aiTEAMS_SYS::IsPredefinedTeam(const dsSTRID *teamID)
{
    dsSTRID players;
    if (aiTEAMS_SYS::PlayersTeamID(&players)->id == teamID->id)
        return true;

    dsSTRID neutral;
    gsTEAMS::NeutralTeamID(&neutral);
    if (neutral.id == teamID->id)
        return true;

    return false;
}
