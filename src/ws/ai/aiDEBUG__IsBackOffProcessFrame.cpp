// ws-engine ai08 (a8_debug.cpp): aiDEBUG::IsBackOffProcessFrame @0x831949C0 (const).
// True when the brain's per-frame debug processing should be skipped: the brain has no model
// object or is locked; or (in off-AI mode) it is not the watched brain, or movement-watch is off.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"   // isOffAI / dbg_isMoveWatchee / _pBrainDebugWatchee
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/anim/animINST.h"

int aiDEBUG::IsBackOffProcessFrame() const
{
    aiBRAIN *brain = pBrain;
    if (!brain->pInst->pObj || brain->nmbLocks > 0)
        return 1;
    if (!isOffAI)
        return 0;
    if (brain != _pBrainDebugWatchee)
        return 1;
    return !dbg_isMoveWatchee.value;
}
