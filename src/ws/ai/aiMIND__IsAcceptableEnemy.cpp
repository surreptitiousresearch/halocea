#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"

extern aiPLANNER *aiPlanner; // ai08 planner singleton

// aiMIND::IsAcceptableEnemy @ 0x83227CC8
// ?IsAcceptableEnemy@aiMIND@@QAA_NQAVaiWATCHER@@@Z
//
// True when the unit watched by `cand` is on a team hostile to this mind's brain.
bool aiMIND::IsAcceptableEnemy(aiWATCHER *cand)
{
    const propGAME_INFO_TEAM *candTeam = cand->GetTeamID();

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
    const propGAME_INFO_TEAM *myTeam = brain->GetTeamID();

    aiTEAMS_SYS *teams = reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams);
    return teams->IsEnemy(*myTeam, *candTeam);
}
