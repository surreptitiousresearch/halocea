// aiTEAMS_SYS::IsNeutralTeam @ 0x83172F68
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/gs/gsTEAMS.h"

bool aiTEAMS_SYS::IsNeutralTeam(const dsSTRID *teamID)
{
    dsSTRID neutral;
    gsTEAMS::NeutralTeamID(&neutral);
    return teamID->id == neutral.id;
}
