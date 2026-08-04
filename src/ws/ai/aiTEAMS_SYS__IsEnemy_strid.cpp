// aiTEAMS_SYS::IsEnemy(const dsSTRID&, const dsSTRID&) @ 0x83173050
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/gs/gsTEAMS.h"

bool aiTEAMS_SYS::IsEnemy(const dsSTRID &teamIDFrom, const dsSTRID &teamIDTo) const
{
    return gsTEAMS::Instance().GetAttitude(teamIDFrom, teamIDTo) == gsTEAMS::ATT_ENEMY;
}
