#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiSTATISTICS::NoticeUnitCreated(const aiBRAIN*) @ 0x832AA418
// ?NoticeUnitCreated@aiSTATISTICS@@QAAXPBVaiBRAIN@@@Z
//
// A brain just entered play: record it as moving from the (invalid) "no team" into its
// current team via NoticeTeamChanged.
void aiSTATISTICS::NoticeUnitCreated(const aiBRAIN *brain)
{
    dsSTRID newTeam;
    newTeam.id = brain->GetTeamID()->team.id;
    dsSTRID oldTeam; // default: invalid/empty interned id
    NoticeTeamChanged(brain, oldTeam, newTeam);
}
