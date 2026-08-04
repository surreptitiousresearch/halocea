#include "../../headers/ws/ai/aiEXIFACE.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h"

// aiEXIFACE::IsTeamOppositeTo @ 0x82D31638
// ?IsTeamOppositeTo@aiEXIFACE@@QAA_NABVdsSTRID@@0@Z
// Hostility test between two interned team ids. When the planner interface is present it owns
// the answer (virtual IsEnemy); otherwise apply the OPP/NEUTRAL fallback rules.
bool aiEXIFACE::IsTeamOppositeTo(const dsSTRID &team, const dsSTRID &teamTo)
{
    if (aiPlanIFace)
        return aiPlanIFace->IsEnemy(team, teamTo);

    // Fallback: the OPP team opposes everything except itself.
    if (TEAM_OPP.id == team.id)
        return team.id != teamTo.id;

    // NEUTRAL opposes nothing.
    dsSTRID neutral;
    if (gsTEAMS::NeutralTeamID(&neutral)->id == team.id)
        return false;

    // Any other team opposes only OPP.
    return TEAM_OPP.id == teamTo.id;
}
