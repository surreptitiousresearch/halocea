#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiPLANNER.h" // aiPlanner->wah

extern aiPLANNER *aiPlanner; // ai08 planner singleton (aiEXIFACE.h)

// aiFILT_ENEMY::Start(const aiBRAIN*) @ 0x8321FB08
// ?Start@aiFILT_ENEMY@@QAAXPBVaiBRAIN@@@Z
// Clear the working set and refill it with a FEE for every watcher the planner's aiWAH_SYS holds,
// filtered by the ai08-internal "Regular" include predicate bound to `brain`.
void aiFILT_ENEMY::Start(const aiBRAIN *brain)
{
    aiFILT_ENEMY_IsIncludeFEECheck_Regular checker;
    // decompiler stores brain at the object's +0x4 (the derived pBrain slot).
    checker.pBrain = const_cast<aiBRAIN *>(brain);

    arr.Clear();

    checker.__vftable = &aiFILT_ENEMY_IsIncludeFEECheck_Regular_vftable;
    aiPlanner->wah.PutAllFEEs(arr, &checker);
}
