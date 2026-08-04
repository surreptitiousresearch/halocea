#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPLANNER_boundary.h" // gsAppState, gsGetPlayer
#include "../../headers/ws/ai/aiCON_CB_globals.h"   // aiPLANNER *aiPlanner
#include "../../headers/ws/ds/dsTSTRING.h"

// aiMIND::CanSetupInitial @ 0x83229730
// ?CanSetupInitial@aiMIND@@IAA_NXZ  (protected)
//
// Decide whether the unit's one-time "initial setup" pass may run now. Each early-out records a
// human-readable rejection reason on the brain's debug block. Setup is blocked while the body has
// not yet initialised, while the game is still loading, or before a player exists; otherwise it is
// allowed if the brain is flagged force-setup (gen bit 7) or the planner accepts it.
bool aiMIND::CanSetupInitial()
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    brain->dbg.reasonRejectWaitInitialSetup = dsTSTRING<char>("no reason");

    // gen bit 6 — body not yet initialised.
    if ((((unsigned int)brain->st.pointee->gen.val >> 6) & 1) != 0)
    {
        brain->dbg.reasonRejectWaitInitialSetup = dsTSTRING<char>("wait body init");
        return false;
    }

    // game-active bit (0x2) still clear → game is loading.
    if (((~gsAppState >> 1) & 1) != 0)
    {
        brain->dbg.reasonRejectWaitInitialSetup = dsTSTRING<char>("game loading");
        return false;
    }

    if (!gsGetPlayer(0))
    {
        brain->dbg.reasonRejectWaitInitialSetup = dsTSTRING<char>("wait player");
        return false;
    }

    // gen bit 7 — force initial setup; otherwise defer to the planner.
    return ((((unsigned int)brain->st.pointee->gen.val >> 7) & 1) != 0)
        || aiPlanner->CanSetupInitial(brain, &brain->dbg.reasonRejectWaitInitialSetup);
}
