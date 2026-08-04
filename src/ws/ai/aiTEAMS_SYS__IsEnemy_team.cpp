// aiTEAMS_SYS::IsEnemy(const propGAME_INFO_TEAM&, const propGAME_INFO_TEAM&) @ 0x831730A0
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/gs/gsTEAMS.h"

bool aiTEAMS_SYS::IsEnemy(const propGAME_INFO_TEAM &teamFrom, const propGAME_INFO_TEAM &teamTo) const
{
    dsSTRID from;
    dsSTRID to;
    from.id = teamFrom.team.id;
    to.id = teamTo.team.id;
    return gsTEAMS::Instance().GetAttitude(from, to) == gsTEAMS::ATT_ENEMY;
}
