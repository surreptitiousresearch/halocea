#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiPLANNER::SetTeamsAttitude(const dsTSTRING<char>&, const dsTSTRING<char>&, gsTEAMS::ATTITUDE)
//   @ 0x831738B0  ?SetTeamsAttitude@aiPLANNER@@IAA_NABV?$dsTSTRING@D@@0W4ATTITUDE@gsTEAMS@@@Z  (protected)
// Set the attitude of team `teamIDFrom` toward `teamIDTo`. Both must be gs-registered; logs the
// change (or failure) and returns whether it took.
bool aiPLANNER::SetTeamsAttitude(const dsTSTRING<char> &teamIDFrom, const dsTSTRING<char> &teamIDTo,
                                 gsTEAMS::ATTITUDE att)
{
    dsSTRID fromId(teamIDFrom.pBuffer->str, false);
    if (!gsTEAMS::Instance().IsTeamRegistered(fromId))
    {
        _apLog("~AI,Teams,Ssl,Error~Team \"%s\" is not registered.", teamIDFrom.pBuffer->str);
        return false;
    }

    dsSTRID toId(teamIDTo.pBuffer->str, false);
    if (!gsTEAMS::Instance().IsTeamRegistered(toId))
    {
        _apLog("~AI,Teams,Ssl,Error~Team \"%s\" is not registered.", teamIDTo.pBuffer->str);
        return false;
    }

    const char *attName = "";
    if (att == gsTEAMS::ATT_NEUTRAL)
        attName = "NEUTRAL";
    else if (att == gsTEAMS::ATT_ENEMY)
        attName = "ENEMY";
    else if ((unsigned int)att < (gsTEAMS::ATT_FRIEND | gsTEAMS::ATT_ENEMY))
        attName = "FRIEND";

    if (gsTEAMS::Instance().SetAttitude(fromId, toId, att))
    {
        _apLog("~AI,Teams,Ssl~Attitude: \"%s\"==[%s]==>\"%s\".",
               teamIDFrom.pBuffer->str, attName, teamIDTo.pBuffer->str);
        return true;
    }

    _apLog("~AI,Teams,Ssl,Error~Attitude \"%s\"==[%s]==>\"%s\" wasn't set.",
           teamIDFrom.pBuffer->str, attName, teamIDTo.pBuffer->str);
    return false;
}
