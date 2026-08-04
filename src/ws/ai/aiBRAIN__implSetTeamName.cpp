#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO.h"
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiBRAIN::implSetTeamName(const dsTSTRING<char>&) @ 0x83173418
// ?implSetTeamName@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z  (virtual)
// SSL hook: reassign the brain's team from the named team `sTeam`. Requires the actor to own a
// game-info block and the name to be a non-empty, gs-registered team; otherwise logs and no-ops.
void aiBRAIN::implSetTeamName(const dsTSTRING<char> &sTeam)
{
    propGAME_INFO *gameInfo = propGameInfo();
    if (!gameInfo)
    {
        _apLog("~AI,Teams,Ssl,Error~%s:actor has no game info => can't SetTeamName()", names.nameInst);
        return;
    }

    if (sTeam.pBuffer->strLen == 0)
    {
        _apLog("~AI,Teams,Ssl,Error~%s:Attempt to setup empty team via SetTeamName()", names.nameInst);
        return;
    }

    dsSTRID team(sTeam.pBuffer->str, false);
    if (gsTEAMS::Instance().IsTeamRegistered(team))
    {
        gameInfo->SetTeamID(team);
    }
    else
    {
        _apLog("~AI,Teams,Ssl,Error~%s:Attempt to setup unregistered team '%s'",
               names.nameInst, sTeam.pBuffer->str);
    }
}
