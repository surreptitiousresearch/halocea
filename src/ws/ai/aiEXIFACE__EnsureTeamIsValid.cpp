#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// ap subsystem logging sink (printf-style) — boundary.
extern void _apLog(const char *fmt, ...);

// aiEXIFACE::EnsureTeamIsValid @ 0x82D31590
// ?EnsureTeamIsValid@aiEXIFACE@@QAAXAAVdsSTRID@@@Z
// If `team` is not a predefined team and (when the planner interface is available) not
// registered with gsTEAMS, warn and fall back to the neutral team id.
void aiEXIFACE::EnsureTeamIsValid(dsSTRID &team)
{
    if (aiTEAMS_SYS::IsPredefinedTeam(&team))
        return;

    if (aiPlanIFace)
    {
        if (gsTEAMS::Instance().IsTeamRegistered(team))
            return;
        _apLog("~Teams,Error~Unregistered teamID(\"%s\"). Changed to NEUTRAL.", team.CStr());
    }
    else
    {
        _apLog("~Teams,Error~Unable to check for \"%s\" team registration. Changed to NEUTRAL.",
               team.CStr());
    }

    dsSTRID neutral;
    team.id = gsTEAMS::NeutralTeamID(&neutral)->id;
}
