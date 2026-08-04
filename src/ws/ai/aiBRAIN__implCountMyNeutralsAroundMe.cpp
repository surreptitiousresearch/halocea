#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // full aiSTATUS (csPosBody)
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// The ai08 planner singleton (aiEXIFACE.h). Declared locally to avoid its header's conflicting
// gsTEAMS boundary definition.
extern aiPLANNER *aiPlanner;

// aiBRAIN::implCountMyNeutralsAroundMe(float) @ 0x83173698
// ?implCountMyNeutralsAroundMe@aiBRAIN@@UAAHM@Z  (virtual)
// SSL hook: count neutral units within `radius` of this brain's own body position. Returns 0 when
// the SSL call is in a common back-off state or the planner is not up.
int aiBRAIN::implCountMyNeutralsAroundMe(float radius)
{
    dsTSTRING<char> callName("CountMyNeutralsAroundMe()");
    dsFLAGS<SSL_COMMON_BACKOFF, int> noFlags{};
    if (IsSSLCommonBackOff(callName, noFlags) || !aiPlanner)
        return 0;

    const propGAME_INFO_TEAM *myTeam = GetTeamID();
    aiSTATUS *status = st.pointee;
    m3dV myPos = status->cs.posBody; // st->cs.posBody (CS@0x20, posBody@+0)

    aiTEAMS_SYS *teams = reinterpret_cast<aiTEAMS_SYS *>(&aiPlanner->teams);
    return teams->CountMyNeutralsNearPoint(*myTeam, myPos, radius);
}
