#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::implGetTeamName() @ 0x83173630
// ?implGetTeamName@aiBRAIN@@UAA?AV?$dsTSTRING@D@@XZ  (virtual, returns by value)
// SSL hook: the brain's team id as a fresh string.
dsTSTRING<char> aiBRAIN::implGetTeamName()
{
    dsSTRID team;
    team.id = GetTeamID()->team.id;

    dsTSTRING<char> result;
    result.pBuffer = nullptr;
    result.UnsafeInit(team.CStr(), -1, 0);
    return result;
}
