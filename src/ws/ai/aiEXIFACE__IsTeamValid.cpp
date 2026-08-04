#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::IsTeamValid @ 0x82D316F0
// ?IsTeamValid@aiEXIFACE@@QAA_NABV?$dsTSTRING@D@@@Z
bool aiEXIFACE::IsTeamValid(const dsTSTRING<char> &teamID)
{
    dsSTRID id(teamID, false); // intern (existOnly = false)
    if (aiTEAMS_SYS::IsPredefinedTeam(&id))
        return true;
    return gsTEAMS::Instance().IsTeamRegistered(id);
}
