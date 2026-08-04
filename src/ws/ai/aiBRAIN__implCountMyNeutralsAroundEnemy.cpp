#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // full aiENEMY_BASE (GetPeeker)
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// The ai08 planner singleton (aiEXIFACE.h). Declared locally to avoid its header's conflicting
// gsTEAMS boundary definition.
extern aiPLANNER *aiPlanner;

// aiBRAIN::implCountMyNeutralsAroundEnemy(float) @ 0x83173790
// ?implCountMyNeutralsAroundEnemy@aiBRAIN@@UAAHM@Z  (virtual)
// SSL hook: count neutral units within `radius` of this brain's current enemy's body position.
// Returns 0 when the SSL call is backed off, the planner is down, or there is no live enemy.
int aiBRAIN::implCountMyNeutralsAroundEnemy(float radius)
{
    dsTSTRING<char> callName("CountMyNeutralsAroundEnemy()");
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (IsSSLCommonBackOff(callName, noFlags))
        return 0;
    if (!aiPlanner)
        return 0;

    aiENEMY_BASE *enemy = mind.enemy.pointee;
    if (!enemy)
        return 0;

    aiWATCHER *peeker = enemy->GetPeeker();
    if (!peeker)
        return 0;

    const propGAME_INFO_TEAM *myTeam = GetTeamID();
    m3dV enemyPos;
    peeker->GetBodyPos(&enemyPos); // decompiler: virtual vtbl+0x34 on the peeker

    aiTEAMS_SYS *teams = reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams);
    return teams->CountMyNeutralsNearPoint(*myTeam, enemyPos, radius);
}
