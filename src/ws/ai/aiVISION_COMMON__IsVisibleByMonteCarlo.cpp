#include "../../headers/ws/ai/aiVISION_COMMON.h"
#include "../../headers/ws/ai/aiBRAIN.h"           // pBrain->st.pointee->peekerSelf
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"   // peekerSelf->GetTeamID
#include "../../headers/ws/ai/aiPLANNER.h"         // aiPlanner->stat
#include "../../headers/ws/ai/aiSTATISTICS.h"      // GetActiveUnitsCnt
#include "../../headers/ws/ai/aiCON_CB_globals.h"  // aiPLANNER *aiPlanner
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/m3d/m3dRND_GEN.h"

extern m3dRND_GEN m3dRndGen; // ?m3dRndGen@@3Vm3dRND_GEN@@A — shared RNG   boundary

// aiVISION_COMMON::IsVisibleByMonteCarlo @ 0x832915D8
// ?IsVisibleByMonteCarlo@aiVISION_COMMON@@MAA_NXZ
// Protected virtual: a probabilistic "already visible" short-circuit. Only fires once the latest
// deferred test has fully resolved (>= 100%) and the viewer's team fields at least 3 active units;
// then it returns visible with ~80% probability.
bool aiVISION_COMMON::IsVisibleByMonteCarlo()
{
    if (viewLatestTest < 100.0f)
        return false;

    aiWATCHER_BRAIN *peeker = pBrain->st.pointee->peekerSelf;
    propGAME_INFO_TEAM *team = peeker->GetTeamID();

    aiSTATISTICS *stat = (&aiPlanner->stat);
    if (stat->GetActiveUnitsCnt(team->team) < 3)
        return false;

    return m3dRndGen.RndIntMax(100) < 80;
}
