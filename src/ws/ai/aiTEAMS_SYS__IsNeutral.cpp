// aiTEAMS_SYS::IsNeutral(const dsSTRID&, const dsSTRID&) @ 0x83173140
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"

bool aiTEAMS_SYS::IsNeutral(const dsSTRID &teamFrom, const dsSTRID &teamTo) const
{
    return gsTEAMS::Instance().GetAttitude(teamFrom, teamTo) == gsTEAMS::ATT_NEUTRAL;
}

// aiTEAMS_SYS::IsNeutral(const propGAME_INFO_TEAM&, const propGAME_INFO_TEAM&) const @ 0x831734D8
// ?IsNeutral@aiTEAMS_SYS@@QBA_NABVpropGAME_INFO_TEAM@@0@Z
// True when the gs registry rates teamFrom's attitude toward teamTo as neutral.
bool aiTEAMS_SYS::IsNeutral(const propGAME_INFO_TEAM &teamFrom, const propGAME_INFO_TEAM &teamTo) const
{
    dsSTRID from;
    from.id = teamFrom.team.id;
    dsSTRID to;
    to.id = teamTo.team.id;
    return gsTEAMS::Instance().GetAttitude(from, to) == gsTEAMS::ATT_NEUTRAL;
}
