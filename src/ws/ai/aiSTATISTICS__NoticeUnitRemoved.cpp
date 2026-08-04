#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiSTATISTICS::NoticeUnitRemoved(const aiBRAIN*) @ 0x832AA480
// ?NoticeUnitRemoved@aiSTATISTICS@@QAAXPBVaiBRAIN@@@Z
//
// A brain left play: record it as moving from its current team into the (invalid) "no team"
// via NoticeTeamChanged, which decrements the team's live-unit count.
void aiSTATISTICS::NoticeUnitRemoved(const aiBRAIN *brain)
{
    dsSTRID oldTeam;
    oldTeam.id = brain->GetTeamID()->team.id;
    dsSTRID newTeam; // default: invalid/empty interned id
    NoticeTeamChanged(brain, oldTeam, newTeam);
}
